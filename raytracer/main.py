import numpy as np
import torch
from torch.utils.data import Dataset, DataLoader
import torch.nn as nn
import torch.optim as optim
from torchvision.io import read_image
from PIL import Image
#%matplotlib inline
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import torchvision.transforms as T

# check if gpu available
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
print(device)

# custom dataset - length 60 000, returns watermarked and watermark free image
class Coles(Dataset):
  def __init__(self, watermark_free, watermarked, transform = None, target_transform = None):
    #self.watermark_free = Image.open("CLWD/train/Watermark_image" + watermark_free)
    #self.img_dir = watermarked
    self.transform = transform
    self.target_transform = target_transform
 # C:\Users\Cole\Desktop/CLWD/
  def __getitem__(self, idx):
    img_path = r"C:\Users\Cole\Desktop/CLWD/CLWD/train/Watermarked_image/" + str(idx + 1) + ".jpg"
    watermarked = Image.open(img_path).convert('RGB')
    img_path = r"C:\Users\Cole\Desktop/CLWD/CLWD/train/Watermark_free_image/" + str(idx + 1) + ".jpg"
    watermark_free = Image.open(img_path).convert('RGB')
    
    # resize = T.Resize(size = (256, 256))

    

    transform_one = T.ToTensor()
    img = transform_one(watermarked)
    label = transform_one(watermark_free)

    # img = resize(img)
    # label = resize(label)

    # watermarked.cuda()
    # watermark_free.cuda()


    return img, label

  def __len__(self):
    #hardcoded for now
    return 60_000

Train_dataset = Coles("", "")


class Encoder(nn.Module):
  def __init__(self):
    super().__init__()
    #takes in 64*64 (or as much as I reasonably can) by 3 channels
    self.encoder = nn.Sequential(
        nn.Conv2d(3, 8, 3, stride = 2, padding = 1),
        nn.LeakyReLU(0.2),
        nn.Conv2d(8, 16, 3, stride = 2, padding = 1),
        nn.LeakyReLU(0.2),
        nn.Conv2d(16, 32, 7),
        #nn.LeakyReLU(0.2),
        #nn.Conv2d(32, 64, 2, stride = 2, padding = 1),

    )
        # this may not be perfect but this is how i think it is right now
        # conv2d(in_channels, out_channels, kernel size, stride, padding)
        # in channels is the number of channels that are input, for this case 3 because R, G, B
        # out channels is number of output(width of the matrix?), will be batch size - number of images
        # kernel size, area of sqare (or rectangle if tuple is provided) that is used to convolve pixel
        # kernel size will probably just be 3, meaning thagt if x is the convoluted pixel then it would take from pixels like this:
        # PPP
        # PxP
        # PPP
        # stride is how many pixels it skips, should probably just be 1
        # padding is how much it leaves off the edges, should be 1
    # self.pool = nn.MaxPool2d(2, stride=2, return_indices=True)
    # self.unpool = nn.MaxUnpool2d(2, stride=2)

    self.decoder = nn.Sequential(
        #nn.Conv2d(64, 32, 2, stride = 2, padding = 1),
        #nn.LeakyReLU(0.2),
        nn.ConvTranspose2d(32, 16, 7),
        nn.LeakyReLU(0.2),
        nn.ConvTranspose2d(16, 8, 3, stride = 2, padding = 1, output_padding = 1),
        nn.LeakyReLU(0.2),
        nn.ConvTranspose2d(8, 3, 3, stride = 2, padding = 1, output_padding = 1),
        nn.Sigmoid()

    )
  # output
  def forward(self, x):
    #print("began")
    encoded = self.encoder(x)

    # output, indices = self.pool(encoded)
    # # This call will not work without specifying output_size
    # unpooled = self.unpool(output, indices)

    #print(encoded)
    decoded = self.decoder(encoded)
    return decoded

# initializing the model
model = Encoder()
criterion = nn.MSELoss()
model.to(device)
criterion.to(device)
optimizer = torch.optim.Adam(model.parameters(), lr = 1e-1, weight_decay = 1e-4)

losses = []
# move to gpu - im not gonna put this in yet until all other bugs are worked out
# so as to not intrudce more until current ones are fixed

train_dataloader = DataLoader(Train_dataset, batch_size=4, shuffle=True)
#nvidia-smi
#transform = T.Resize(size = (256,256))
print(device)
# Training loop
transform = T.ToPILImage()
if __name__ == '__main__':
  epochs = 12
  for e in range(epochs):
      counter = 0
      running_loss = 0
      for images, labels in train_dataloader:
          counter += 1
          if counter % (37.5*4) == 0:
            print("training at epoch ", e, " ", (counter / (3750 * 4)) * 100, "% done")
          # print(images.shape)
          # images = images.reshape(-1, 256*256) * 0.00390625
          # print(images.shape)
          # print(images)
          # images = images.float()
          # print(images.shape)
          # print(images)

          images = images.to(device)
          labels = labels.to(device)

          #print(len(images))
          #batch_size, channels, depth, height, width
          #images = torch.reshape(images, (len(images), 3, 256, 256))
          #print(images.shape)


          # img = images[0]
          # pic122 = transform(img)
          # pic122.show()

          # img123 = labels[0]
          # pic1 = transform(img123)
          # pic1.show()

          #print(images)
          # print(images.shape)
          # print(labels.shape)

          # Training pass

          
          output = model(images)
          print(output.shape)
          output.to(device)

          optimizer.zero_grad()
          loss = criterion(output, labels)
          #print(loss)
          loss.backward()
          optimizer.step()
          
          running_loss += loss.item()

      losses.append((running_loss)/15000)
      torch.save({
              'epoch': e,
              'model_state_dict': model.state_dict(),
              'optimizer_state_dict': optimizer.state_dict(),
              'loss': running_loss/15000,
              }, f"five-ice_more_epoch-{e}.pth")
      
print(losses)

