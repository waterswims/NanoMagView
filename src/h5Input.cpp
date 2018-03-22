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
    H5Sget_simple_extent_dims(Hspace_id, &Hsize, NULL);
    H5Sget_simple_extent_dims(Tspace_id, &Tsize, NULL);

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

    // Strip Array of Boundaries
    int nXBounds = 0, nYBounds = 0, nZBounds = 0;
    for(unsigned int i = 0; i < Lsize[0]/2; i++)
    {
        int y = Lsize[1]/2;
        int z = Lsize[2]/2;
        int in_ind = 3 * (Lsize[2] * (Lsize[1] * i + y) + z);
        if(spinsIn[in_ind]==0 && spinsIn[in_ind+1]==0 && spinsIn[in_ind+2]==0)
        {
            nXBounds++;
        }
    }
    for(unsigned int i = 0; i < Lsize[1]/2; i++)
    {
        int x = Lsize[0]/2;
        int z = Lsize[2]/2;
        int in_ind = 3 * (Lsize[2] * (Lsize[1] * x + i) + z);
        if(spinsIn[in_ind]==0 && spinsIn[in_ind+1]==0 && spinsIn[in_ind+2]==0)
        {
            nYBounds++;
        }
    }
    for(unsigned int i = 0; i < Lsize[2]/2; i++)
    {
        int x = Lsize[0]/2;
        int y = Lsize[1]/2;
        int in_ind = 3 * (Lsize[2] * (Lsize[1] * x + y) + i);
        if(spinsIn[in_ind]==0 && spinsIn[in_ind+1]==0 && spinsIn[in_ind+2]==0)
        {
            nZBounds++;
        }
    }
    hsize_t strLsize[4];
    int strLsize_tot = 1, strTsize;
    strLsize[0] = Lsize[0] - 2*nXBounds;
    strLsize[1] = Lsize[1] - 2*nYBounds;
    strLsize[2] = Lsize[2] - 2*nZBounds;
    strLsize[3] = 3;
    for(int i=0; i<3; i++) {strLsize_tot*=strLsize[i];}
    strTsize = strLsize_tot * strLsize[3];
    float* strSpinsIn = alloc_1darr<float>(strTsize);
    for(unsigned int x = 0; x < strLsize[0]; x++)
    {
        int x_old = x + nXBounds;
        for(unsigned int y = 0; y < strLsize[1]; y++)
        {
            int y_old = y + nYBounds;
            for(unsigned int z = 0; z < strLsize[2]; z++)
            {
                int z_old = z + nZBounds;
                int in_ind = 3 * (strLsize[2] * (strLsize[1] * x + y) + z);
                int in_ind_old = 3 * (Lsize[2] * (Lsize[1] * x_old + y_old) + z_old);
                for(int i = 0; i < 3; i++)
                {strSpinsIn[in_ind+i] = spinsIn[in_ind_old+i];}
            }
        }
    }

    // Create the geometry
    geometry =
        VFRendering::Geometry::cartesianGeometry({strLsize[0], strLsize[1], strLsize[2]},
        {-float(strLsize[0]-1)/2.0, -float(strLsize[1]-1)/2.0, -float(strLsize[2]-1)/2.0},
        {float(strLsize[0]-1)/2.0, float(strLsize[1]-1)/2.0, float(strLsize[2]-1)/2.0});
    directions.resize(strLsize_tot);

    for(unsigned int x = 0; x < strLsize[0]; x++)
    {
        for(unsigned int y = 0; y < strLsize[1]; y++)
        {
            for(unsigned int z = 0; z < strLsize[2]; z++)
            {
                int out_ind, in_ind;
                in_ind = 3 * (strLsize[2] * (strLsize[1] * x + y) + z);
                out_ind = strLsize[0] * (strLsize[1] * z + y) + x;
                directions[out_ind] = glm::vec3(strSpinsIn[in_ind],
                    strSpinsIn[in_ind+1], strSpinsIn[in_ind+2]);
            }
        }
    }

    // Deallocate temp arrays
    dealloc_1darr<float>(spinsIn);
    dealloc_1darr<float>(strSpinsIn);
}
