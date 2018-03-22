#include "../include/h5Input.hpp"
#include "../include/array_alloc.hpp"

#include <hdf5.h>
#include <sstream>
#include <iostream>

void h5Input::getFieldTemps(
    std::string fileName,
    std::vector<float> &Ts,
    std::vector<float> &Hs)
{
    // Open the file
    hid_t file_id = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    // Open the T and H datasets
    hid_t Hset_id = H5Dopen2(file_id, "/Hs", H5P_DEFAULT);
    hid_t Tset_id = H5Dopen2(file_id, "/Ts", H5P_DEFAULT);

    // Get the size of the T and H arrays
    hsize_t Tsize, Hsize;
    hid_t Hspace_id = H5Dget_space(Hset_id);
    hid_t Tspace_id = H5Dget_space(Tset_id);
    H5Sget_simple_extent_dims(Hset_id, &Hsize, NULL);
    H5Sget_simple_extent_dims(Tset_id, &Tsize, NULL);

    // Allocate space for the T and H arrays
    float* Hin = alloc_1darr<float>(Hsize);
    float* Tin = alloc_1darr<float>(Tsize);

    // Read in the arrays
    H5Dread(Hset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, Hin);
    H5Dread(Tset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, Tin);

    // Close HDF5
    H5Sclose(Hspace_id);
    H5Sclose(Tspace_id);
    H5Dclose(Hset_id);
    H5Dclose(Tset_id);
    H5Fclose(file_id);

    // Copy to vectors
    Ts.resize(Tsize);
    Hs.resize(Hsize);
    for(unsigned int i = 0; i < Hsize; i++)
    {
        Hs[i] = Hin[i];
        Ts[i] = Tin[i];
    }

    // Deallocate arrays
    dealloc_1darr<float>(Tin);
    dealloc_1darr<float>(Hin);
}

void h5Input::getSpins(
    std::string fileName,
    const int Tind,
    const int Hind,
    VFRendering::Geometry &geometry,
    std::vector<glm::vec3> &directions)
{
    // Open the file
    hid_t file_id = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    // Get dataset name
    std::stringstream dsetStream;
    std::string dsetName;
    dsetStream << "/Sing_Latt/T_" << Tind << "-H_" << Hind;
    dsetStream >> dsetName;

    // Open the lattice dataset
    hid_t dset_id = H5Dopen2(file_id, dsetName.c_str(), H5P_DEFAULT);

    // Get the size of the dataset
    hsize_t Lsize[4];
    hid_t dspace_id = H5Dget_space(dset_id);
    H5Sget_simple_extent_dims(dspace_id, Lsize, NULL);

    // Allocate space
    unsigned int tsize, Lsize_tot = 1;
    for(int i=0; i<3; i++) {Lsize_tot*=Lsize[i];}
    tsize = Lsize_tot * Lsize[3];
    float* spinsIn = alloc_1darr<float>(tsize);

    // Read in the array
    H5Dread(dset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, spinsIn);

    // Close HDF5
    H5Sclose(dspace_id);
    H5Dclose(dset_id);
    H5Fclose(file_id);

    // Create the geometry
    geometry =
        VFRendering::Geometry::cartesianGeometry({Lsize[0], Lsize[1], Lsize[2]},
        {-float(Lsize[0]-1)/2.0, -float(Lsize[1]-1)/2.0, -float(Lsize[2]-1)/2.0},
        {float(Lsize[0]-1)/2.0, float(Lsize[1]-1)/2.0, float(Lsize[2]-1)/2.0});
    directions.resize(Lsize_tot);

    for(unsigned int x = 0; x < Lsize[0]; x++)
    {
        for(unsigned int y = 0; y < Lsize[1]; y++)
        {
            for(unsigned int z = 0; z < Lsize[2]; z++)
            {
                int out_ind, in_ind;
                in_ind = 3 * (Lsize[2] * (Lsize[1] * x + y) + z);
                out_ind = Lsize[0] * (Lsize[1] * z + y) + x;
                directions[out_ind] = glm::vec3(spinsIn[in_ind],
                    spinsIn[in_ind+1], spinsIn[in_ind+2]);
            }
        }
    }
}
