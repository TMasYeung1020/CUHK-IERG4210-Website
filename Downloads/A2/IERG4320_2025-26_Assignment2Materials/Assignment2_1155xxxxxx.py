# <Your student ID>
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
import os
import torch
import torchaudio
from torchaudio import transforms
from torch.utils.data import Dataset, DataLoader
from torch import nn, optim


# Problem 2
def problem_2(df, k=5, t=0.2):
    list = []
    # write your logic here, test is a dataframe
    test = 0
    
    
    return list, test

# Problem 3
def problem_3(filename, column_label):
    # write your logic here, df is a dataframe instead
    df = 0
    

    return df

# Problem 4
def problem_4(path):
    # Don't touch the settings below
    desired_length, n_fft, hop_len, n_mels, top_db = 100000, 1024, 256, 32, 80
    list_of_mel_spec_db = []
    # retrieve the list of files under path
    file_names = os.listdir(path)
    
    for file in file_names:
        # write your logic here: load audio file
        
        
        
        # get the current length of the waveform
        current_length = waveform.size(1)  # Size (channels, length)
    
        # write your logic here:
        # truncate the waveform, or pad the waveform with zeros
        
        
        
        # write your logic here: define mel spec with the settings
        mel_transform = transforms.MelSpectrogram(
            sample_rate=1,
            n_fft=1,
            hop_length=1,
            n_mels=1
        )
        
        # write your logic here: apply the transform to the waveform
        mel_spec = 1
        
        # write your logic here: convert to decibels with the settings
        mel_spec_db = transforms.AmplitudeToDB(top_db=1)(mel_spec)
        
        list_of_mel_spec_db.append(mel_spec_db)
        
    # convert the list to a tensor
    # assume all spec have the same shape
    mel_spec_tensor = torch.stack(list_of_mel_spec_db)

    return mel_spec_tensor

# Problem 5
class MyDataset(Dataset):
	def __init__(self, tensors):
		self.tensors = tensors
	def __len__(self):
		return len(self.tensors)
	def __getitem__(self, idx):
		return self.tensors[idx]

# write your autoencoder structure here
class AE(nn.Module):
    def __init__(self):
        super(AE, self).__init__()
        self.encoder = nn.Sequential(

        )

        self.decoder = nn.Sequential(

        )

    def forward(self, x):
        encoded = self.encoder(x)
        decoded = self.decoder(encoded)
        return decoded, encoded

def problem_5(tensor_file):
    loaded_tensor = torch.load(tensor_file, weights_only=True)
    dataset = MyDataset(loaded_tensor)
    dataloader = DataLoader(dataset, batch_size=25, shuffle=False)
    
    model = AE()
    loss_function = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-3, weight_decay=1e-8)
    epochs = 20
    losses = []
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.to(device)
    
    for epoch in range(epochs):
        loss1 = []
        for audio in dataloader:
            audio = audio.view(-1, 2 * 32 * 391).to(device)
            reconstructed = model(audio)[0]
            loss = loss_function(reconstructed, audio)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            loss1.append(loss.item())
            
        losses.append(sum(loss1) / len(loss1))
            
    return losses

# Problem 6
def problem_6(tensor_file):
    # write your logic here   
    losses = []
      
    
    return losses


if __name__ == "__main__":
    # Testing: Problem 2
    df = pd.read_csv("problem2.csv")
    list, test = problem_2(df, k=5, t=0.2)
    for item in list:
        print("Segment: ", item.shape)
    print("Testing: ", test.shape)


    # Testing: Problem 3
    df = problem_3("problem3.csv","color")
    print(df)
   
   
    # Testing: Problem 4
    tensor = problem_4("TRAIN")
    print("Type of tensor:", type(tensor))
    print("Shape of tensor:", tensor.shape)
    torch.save(tensor, "problem_4.pt")

    
    # Testing: Problem 5
    losses = []
    losses = problem_5("problem_4.pt")
    i = 1
    for loss in losses:
        print(f"Epoch {i}/20, Loss: {loss:.6f}")
        i += 1
    
    
    # Testing: Problem 6
    losses = []
    losses = problem_6("problem_4.pt")
    i = 1
    for loss in losses:
        print(f"Epoch {i}/20, Loss: {loss:.6f}")
        i += 1
