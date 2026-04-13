import triton
import triton.language as tl
import torch
import math

@triton.jit
def online_softmax_kernel(
    x_ptr,
    y_ptr,
    batch,
    seq_len,
    BLOCK_SIZE: tl.constexpr
):
    pid = tl.program_id(0)
    if pid >= batch:
        return
    x_row = x_ptr + pid * seq_len
    y_row = y_ptr + pid * seq_len

    max_x = -float('inf')
    exp_sum = 0.0
    for start in range(0, seq_len, BLOCK_SIZE):
        offs = start + tl.arange(0, BLOCK_SIZE)
        mask = offs < seq_len

        x_block = tl.load(x_row + offs, mask = mask)
        max_block = tl.max(x_block)
        max_new = tl.maximum(max_block, max_x)

        exp_sum = exp_sum * tl.exp(max_x - max_new) + tl.sum(tl.exp(x_block - max_new), axis = 0)
        max_x = max_new

    for start in range(0, seq_len, BLOCK_SIZE):
        offs = start + tl.arange(0, BLOCK_SIZE)
        mask = offs < seq_len

        x_block = tl.load(x_row + offs, mask = mask)
        y_block = tl.exp(x_block - max_x) / exp_sum
        tl.store(y_row + offs, y_block, mask = mask)

def online_softmax(
    x: torch.Tensor,
    y: torch.Tensor,
    block_size: int
):
    batch, seq_len = x.shape
    grid = (batch,)
    online_softmax_kernel[grid](x, y, batch, seq_len, BLOCK_SIZE=block_size)

if __name__ == "__main__":
    torch.manual_seed(42)
    batch_size = 4
    seq_len = 8
    x = torch.randn(batch_size, seq_len, device='cuda')
    y = torch.zeros(batch_size, seq_len, device='cuda')
    online_softmax(x, y, 4)
    z = torch.softmax(x, dim=-1)
    diff = torch.abs(y - z);
    print(diff)