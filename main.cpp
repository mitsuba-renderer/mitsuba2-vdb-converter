#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/ValueTransformer.h>

#include <fstream>
#include <iostream>

void printUsage() {
    std::cout << "Usage: volconv filename.vdb [gridname (optional)]" << std::endl;
}

template<typename T>
void write(std::ofstream &f, T data) {
    f.write(reinterpret_cast<const char *>(&data), sizeof(data));
}

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: Too few arguments" << std::endl;
        printUsage();
        return -1;
    }

    std::string filename = argv[1];
    std::string gridname = "";
    if (argc == 3) {
        gridname = argv[2];
    }

    std::string outputformat = "binary";
    if (argc == 4) {
        outputformat = argv[3];
    }


    openvdb::initialize();
    openvdb::io::File file(filename);
    file.open();
    // Loop over all grids in the file and retrieve a shared pointer
    // to the one named "LevelSetSphere".  (This can also be done
    // more simply by calling file.readGrid("LevelSetSphere").)
    openvdb::GridBase::Ptr base_grid;
    for (openvdb::io::File::NameIterator name_iter = file.beginName();
        name_iter != file.endName(); ++name_iter)
    {
        // Read in only the grid we are interested in.
        if (gridname == "" || name_iter.gridName() == gridname) {
            base_grid = file.readGrid(name_iter.gridName());
            if (gridname == "")
                break;
        } else {
            std::cout << "skipping grid " << name_iter.gridName() << std::endl;
        }
    }
    openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(base_grid);


    auto bbox_dim = grid->evalActiveVoxelDim();
    auto bbox = grid->evalActiveVoxelBoundingBox();

    auto ws_min = grid->indexToWorld(bbox.min());
    auto ws_max = grid->indexToWorld(bbox.max() - openvdb::Vec3R(1, 1, 1));
    openvdb::tools::GridSampler<openvdb::FloatGrid, openvdb::tools::BoxSampler> sampler(*grid);
    // Compute the value of the grid at fractional coordinates in index space.

    std::vector<float> values;
    for (int k = bbox.min().z(); k < bbox.max().z(); ++k) {
        for (int j = bbox.min().y(); j < bbox.max().y(); ++j) {
            for (int i = bbox.min().x(); i < bbox.max().x(); ++i) {                
                float value = sampler.isSample(openvdb::Vec3R(i, j, k));
                values.push_back(value);
            }
        }
    }
    std::string output_filename = filename.substr(0, filename.find_last_of('.')) + ".vol";
    if (outputformat == "ascii") {
        std::ofstream output_file(output_filename);
        if (!output_file.is_open()) {
            std::cout << "Could not open output file!\n";
            return -1;
        }
        for (size_t i = 0; i < values.size(); ++i) {
            if (i > 0) {
                output_file << ", ";
            }
            output_file << std::to_string(values[i]);
        }
        output_file.close();
    } else if (outputformat == "binary") {
        std::ofstream output_file(output_filename, std::ios::binary);
        if (!output_file.is_open()) {
            std::cout << "Could not open output file!\n";
            return -1;
        }
        output_file << 'V';
        output_file << 'O';
        output_file << 'L';
        uint8_t version = 3;
        write(output_file, version);

        write(output_file, (int32_t) 1); // type
        write(output_file, (int32_t)bbox_dim.x() - 1);
        write(output_file, (int32_t)bbox_dim.y() - 1);
        write(output_file, (int32_t)bbox_dim.z() - 1);
        write(output_file, (int32_t) 1); // #n channels

        float xmin = ws_min.x();
        float ymin = ws_min.y();
        float zmin = ws_min.z();
        float xmax = ws_max.x();
        float ymax = ws_max.y();
        float zmax = ws_max.z();
        write(output_file, xmin);
        write(output_file, ymin);
        write(output_file, zmin);
        write(output_file, xmax);
        write(output_file, ymax);
        write(output_file, zmax);

        for (size_t i = 0; i < values.size(); ++i) {
            write(output_file, values[i]);
        }
        output_file.close();
    } else {
        std::cerr << "Invalid output format: " << outputformat << std::endl;
        printUsage();
        return -1;
    }
    file.close();
}
