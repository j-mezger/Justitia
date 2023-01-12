# Minimal Setup on Cloudlab
## Cloudlab Profile
The file `cloudlab_minimal_setup.rspec` contains a definition of a minimal project.
Go to [CloudLab Profiles](https://www.cloudlab.us/manage_profile.php) and create a new profile based on the aforementioned file.

The profile defines two bare-metal `m510` machines (see [here](https://docs.cloudlab.us/hardware.html) for more info), running Ubuntu 18.04. The two machines are connected via Ethernet.
The profile only describes these resources; to actually get them running, it needs to be instantiated.
After instantiating the profile, we have a running experiment (*instance of a profile*, in Cloudlab terminology).

## Driver installation and project compilation
Then, we need to install OFED drivers, which are needed for RDMA, on **both** of the bare-metal machines we spun up.

Run the script `setup_cloudlab.sh` with your connection string as the argument; for instance:
```
source setup_cloud.sh username@msxxxx.utah.cloudlab.us
```
Repeat this step for both machines.

The script automates the steps outlined in the [Mellanox manual](https://docs.nvidia.com/networking/display/MLNXENv496060LTS/Downloading+Mellanox+OFED).