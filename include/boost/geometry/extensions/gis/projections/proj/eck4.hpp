#ifndef BOOST_GEOMETRY_PROJECTIONS_ECK4_HPP
#define BOOST_GEOMETRY_PROJECTIONS_ECK4_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2017.
// Modifications copyright (c) 2017, Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/aasincos.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace proj
{
    struct eck4 {};

}} //namespace srs::proj

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace eck4
    {

            static const double C_x = .42223820031577120149;
            static const double C_y = 1.32650042817700232218;
            static const double RC_y = .75386330736002178205;
            static const double C_p = 3.57079632679489661922;
            static const double RC_p = .28004957675577868795;
            static const double EPS = 1e-7;
            static const int NITER = 6;

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_eck4_spheroid : public base_t_fi<base_eck4_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;


                inline base_eck4_spheroid(const Parameters& par)
                    : base_t_fi<base_eck4_spheroid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double p, V, s, c;
                    int i;

                    p = C_p * sin(lp_lat);
                    V = lp_lat * lp_lat;
                    lp_lat *= 0.895168 + V * ( 0.0218849 + V * 0.00826809 );
                    for (i = NITER; i ; --i) {
                        c = cos(lp_lat);
                        s = sin(lp_lat);
                        lp_lat -= V = (lp_lat + s * (c + 2.) - p) /
                            (1. + c * (c + 2.) - s * s);
                        if (fabs(V) < EPS)
                            break;
                    }
                    if (!i) {
                        xy_x = C_x * lp_lon;
                        xy_y = lp_lat < 0. ? -C_y : C_y;
                    } else {
                        xy_x = C_x * lp_lon * (1. + cos(lp_lat));
                        xy_y = C_y * sin(lp_lat);
                    }
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double c;

                    lp_lat = aasin(xy_y / C_y);
                    lp_lon = xy_x / (C_x * (1. + (c = cos(lp_lat))));
                    lp_lat = aasin((lp_lat + sin(lp_lat) * (c + 2.)) / C_p);
                }

                static inline std::string get_name()
                {
                    return "eck4_spheroid";
                }

            };

            // Eckert IV
            template <typename Parameters>
            void setup_eck4(Parameters& par)
            {
                par.es = 0.;
            }

    }} // namespace detail::eck4
    #endif // doxygen

    /*!
        \brief Eckert IV projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
            - Pseudocylindrical
            - Spheroid
        \par Example
        \image html ex_eck4.gif
    */
    template <typename CalculationType, typename Parameters = parameters>
    struct eck4_spheroid : public detail::eck4::base_eck4_spheroid<CalculationType, Parameters>
    {
        inline eck4_spheroid(const Parameters& par) : detail::eck4::base_eck4_spheroid<CalculationType, Parameters>(par)
        {
            detail::eck4::setup_eck4(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::proj::eck4, eck4_spheroid, eck4_spheroid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class eck4_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<eck4_spheroid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void eck4_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("eck4", new eck4_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_ECK4_HPP

