import torch
import torch.nn as nn
import math

class SelfAttention:
    def __init__(self, embed_dim):
        super().__init__()
        self.embed_dim = embed_dim
        self.q_projection = nn.Linear(embed_dim, embed_dim)
        self.k_projection = nn.Linear(embed_dim, embed_dim)
        self.v_projection = nn.Linear(embed_dim, embed_dim)
        self.out_linear = nn.Linear(embed_dim, embed_dim)
        self.scale = math.sqrt(embed_dim)
    
    def forward(self, x):
        q_matrix = self.q_projection(x)
        k_matrix = self.k_projection(x)
        v_matrix = self.v_projection(x)
        scores = torch.matmul(q_matrix, k_matrix.transpose(-2, -1)) / self.scale
        attention_weights = torch.softmax(scores, dim = -1)
        output = self.out_linear(torch.matmul(attention_weights, v_matrix))
        return output

if __name__ == "__main__":
    torch.manual_seed(42)
    batch_size = 2
    seq_len = 4
    embed_dim = 8
    x = torch.randn(batch_size, seq_len, embed_dim)
    self_attention = SelfAttention(embed_dim)
    output = self_attention.forward(x)
    print(output)
    