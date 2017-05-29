#ifndef __EOWS_GDAL_RASTER_HPP__
#define __EOWS_GDAL_RASTER_HPP__

// STL
#include <vector>

// GDAL
#include <gdal_priv.h>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace gdal
  {
    // Forward declaration
    class band;
    struct property;

    /*!
     * \brief Defines an interface for handling Raster elements.
     *
     * \todo Currently it handles GeoTIFF. We need be able to handle other Raster formats like HDF
     *
     * \example
     * // Defining raster object
     * eows::gdal::raster rst;
     *
     * std::vector<eows::gdal::property> properties;
     *
     * // Defining a band property with type int8 (0-255)
     * properties.push_back(eows::gdal::property(0, eows::gdal::datatype::uint8));
     *
     * // Creating a new raster in memory
     * rst.create("myraster.tiff", 36, 36, properties);
     *
     * // Retrieving pointer to band in order to fill
     * eows::gdal::band* b = rst.get_band(0);
     *
     * // Fill raster
     * for(int i = 0; i < 36; ++i)
     *   for(int j = 0; j < 36; ++j)
     *     b->set_value(i, j, 0);
     *
     * // Save raster
     * rst.close();
     */
    class raster : private boost::noncopyable
    {
      public:
        /*!
         * \brief It defines a common policies to open a raster object
         */
        enum class access_policy
        {
          read,      //!< Read-Only Access
          write,     //!< Write-Only access
          read_write //!< Read and Write access
        };

        /*!
         * \brief Defines metadata tags for GeoTIFF datasets
         */
        enum class metadata
        {
          document_name,     //!< TIFFTAG_DOCUMENTNAME
          image_description, //!< TIFFTAG_IMAGEDESCRIPTION
          sofware,           //!< TIFFTAG_SOFTWARE
          datetime,          //!< TIFFTAG_DATETIME
          artist,            //!< TIFFTAG_ARTIST
          host_coputer,      //!< TIFFTAG_HOSTCOMPUTER
          copyright,         //!< TIFFTAG_COPYRIGHT
          x_resolution,      //!< TIFFTAG_XRESOLUTION
          y_resolution,      //!< TIFFTAG_YRESOLUTION
          resolution_unit    //!< TIFFTAG_RESOLUTIONUNIT
        };

        raster();

        virtual ~raster();

        void open(const std::string& path, access_policy policy = access_policy::read);

        /*!
         * \brief It tries to create a new dataset in memory
         *
         * \throws eows::gdal::gdal_error When GDAL could not create a dataset
         * \param filename - Dataset name
         * \param col - Raster axis X
         * \param row - Raster axis Y
         * \param properties - Raster Band properties used to describe each band
         */
        void create(const std::string& filename, const std::size_t& col, const std::size_t& row, const std::vector<property>& properties);

        /*!
         * \brief It tries to close raster dataset.
         */
        void close();

        band* get_band(const std::size_t& id) const;

        void set_value(const std::size_t& col, const std::size_t& row, const double& value, const std::size_t id);
        void get_value(const std::size_t& col, const std::size_t& row, const std::size_t id, double* value);

        void transform(const double llx,
                       const double lly,
                       const double urx,
                       const double ury,
                       const double resx,
                       const double resy);


        void set_name(const std::string& name);
        void set_description(const std::string& desc);
        void set_metadata(metadata flag, const std::string& value);
        void set_projection(const std::string& proj_wkt);

        static void get_bands(raster* rst, GDALDataset* dset, std::vector<band*>& bands);
        static const std::string get_metadata_key(metadata flag);
      private:
        access_policy policy_;
        std::size_t col_;
        std::size_t row_;
        std::vector<band*> bands_;
        GDALDataset* dataset_;
        char** metadata_;
    };
  }
}

#endif //__EOWS_GDAL_RASTER_HPP__
