# Minimal Setup on Cloudlab
## Cloudlab Profile
The file `cloudlab_minimal_setup.rspec` contains a definition of a minimal project.
Go to [CloudLab Profiles](https://www.cloudlab.us/manage_profile.php) and create a new profile based on the aforementioned file.

The profile defines two bare-metal `m510` machines (see [here](https://docs.cloudlab.us/hardware.html) for more info), running Ubuntu 18.04. The two machines are connected via Ethernet.
The profile only describes these resources; to actually get them running, it needs to be instantiated.
After instantiating the profile, we have a running experiment (*instance of a profile*, in Cloudlab terminology).

## Driver installation and project compilation
Then, we need to install OFED drivers, which are needed for RDMA, on **both** of the bare-metal machines we spun up.

Run the script `setup_cloudlab.sh` with your username and hosts as the arguments; for instance:
```
source setup_cloud.sh username msxxxx.utah.cloudlab.us
```
Repeat this step for both machines.

The script automates the steps outlined in the [Mellanox OFED driver installation manual](https://docs.nvidia.com/networking/display/MLNXENv496060LTS/Downloading+Mellanox+OFED)
and the instructions on how to [enable RoCEv2 for ConnectX-3 Pro](https://support.mellanox.com/s/article/howto-configure-roce-v2-for-connectx-3-pro-using-mellanox-switchx-switches).

## Running the Justitia Pacer
Pick one of your two nodes to be the *receiver*. SSH to it and use Cloudlab to find out its private IP address (should be either `10.0.1.1` or `10.0.1.2`),
which we will in the following refer to as `<receiver_ip`>
While still on the receiver, run 
```
cd Justitia/rdma_pacer
./pacer 0 <receiver_ip> 1 0
```
and leave this terminal window open.

Open another terminal window and SSH to the other node, which will be the *sender*.
Run 
```
cd Justitia/rdma_pacer
./pacer 1 <receiver_ip> 1 0
```
and leave the terminal window open.