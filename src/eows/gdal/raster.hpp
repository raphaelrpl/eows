#ifndef __EOWS_GDAL_RASTER_HPP__
#define __EOWS_GDAL_RASTER_HPP__

// STL
#include <vector>

// GDAL
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    // Forward declaration
    class band;
    struct property;

    class raster
    {
      public:
        enum class access_policy
        {
          read,
          write,
          read_write
        };

        /*!
         * \brief Defines metadata tags for GeoTIFF datasets
         */
        enum class metadata
        {
          document_name,
          image_description,
          sofware,
          datetime,
          artist,
          host_coputer,
          copyright,
          x_resolution,
          y_resolution,
          resolution_unit
        };

        raster();

        virtual ~raster();

        void open(const std::string& path, access_policy policy = access_policy::read);

        void create(const std::string& filename, const std::size_t& col, const std::size_t& row, const std::vector<property>& properties);

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
