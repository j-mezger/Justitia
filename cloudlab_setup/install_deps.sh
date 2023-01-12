curl -o MLNX_OFED_LINUX-4.9-6.0.6.0-ubuntu18.04-x86_64.iso https://content.mellanox.com/ofed/MLNX_OFED-4.9-6.0.6.0/MLNX_OFED_LINUX-4.9-6.0.6.0-ubuntu18.04-x86_64.iso 
sudo mount -o ro,loop MLNX_OFED_LINUX-4.9-6.0.6.0-ubuntu18.04-x86_64.iso /mnt 
yes | sudo /mnt/mlnxofedinstall

git clone https://github.com/j-mezger/Justitia
cd Justitia
bash setup_X3.sh 
cd rdma_pacer
make