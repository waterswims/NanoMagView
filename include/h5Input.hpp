#ifndef __H5INPUT
#define __H5INPUT

#include <string>
#include <cstring>
#include <vector>
#include <VFRendering/Geometry.hxx>

namespace h5Input
{
    ////////////////////////////////////////////////////////////////////////////
    /// Function which extracts the fields and temperatures for which lattices
    /// exist in a specified HDF5 file.
    ///
    /// /param fileName The name of the input HDF5 file
    /// /param Ts A vector of floats where the temperatures will be placed
    /// /param Hs A vector of floats where the fields will be placed
    ////////////////////////////////////////////////////////////////////////////
    void getFieldTemps(
        std::string fileName,
        std::vector<float> &Ts,
        std::vector<float> &Hs);

    ////////////////////////////////////////////////////////////////////////////
    /// Function which extracts the spin values at a specific field and
    /// temperature
    ///
    /// /param fileName The name of the input HDF5 file
    /// /param Tind The index of the chosen temperature
    /// /param Hind The index of the chosen field
    /// /param geometry The geometry of the lattice
    /// /param directions The directions of the spins
    ////////////////////////////////////////////////////////////////////////////
    void getSpins(
        std::string fileName,
        const int Tind,
        const int Hind,
        VFRendering::Geometry &geometry,
        std::vector<glm::vec3> &directions);
}

#endif
