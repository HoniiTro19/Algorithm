import torch
import torch.nn as nn
import math
import numpy as np

class SelfAttention(nn.Module):
    def __init__(self, embed_dim):
        super().__init__()
        self.embed_dim = embed_dim
        self.q_linear = nn.Linear(embed_dim, embed_dim)
        self.k_linear = nn.Linear(embed_dim, embed_dim)
        self.v_linear = nn.Linear(embed_dim, embed_dim)
        self.out_linear = nn.Linear(embed_dim, embed_dim)
        self.scale = math.sqrt(self.embed_dim)
    
    def forward(self, x):
        # x: [batch_size, seq_len, embed_dim]
        # output: [batch_size, seq_len, embed_dim]
        # attn_weights: [batch_size, seq_len, seq_len]
        Q = self.q_linear(x)
        K = self.k_linear(x)
        V = self.v_linear(x)
        scores = torch.matmul(Q, K.transpose(-2, -1)) / self.scale
        attn_weights = torch.softmax(scores, dim=-1)
        output = torch.matmul(attn_weights, V)
        output = self.out_linear(output)
        return output, attn_weights

if __name__ == '__main__':
    torch.manual_seed(42)
    batch_size = 2;
    seq_len = 4
    embed_dim = 8
    x = torch.randn(batch_size, seq_len, embed_dim)
    self_attention = SelfAttention(embed_dim)
    output = self_attention.forward(x)
    print(output)