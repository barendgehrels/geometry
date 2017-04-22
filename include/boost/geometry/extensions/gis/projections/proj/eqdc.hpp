#ifndef BOOST_GEOMETRY_PROJECTIONS_EQDC_HPP
#define BOOST_GEOMETRY_PROJECTIONS_EQDC_HPP

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

#include <boost/geometry/util/math.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_mlfn.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_msfn.hpp>

namespace boost { namespace geometry
{

namespace srs { namespace proj
{
    struct eqdc {};

}} //namespace srs::proj

namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace eqdc
    {

            static const double EPS10 = 1.e-10;

            struct par_eqdc
            {
                double phi1;
                double phi2;
                double n;
                double rho0;
                double c;
                double en[EN_SIZE];
                int        ellips;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename CalculationType, typename Parameters>
            struct base_eqdc_ellipsoid : public base_t_fi<base_eqdc_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>
            {

                typedef CalculationType geographic_type;
                typedef CalculationType cartesian_type;

                par_eqdc m_proj_parm;

                inline base_eqdc_ellipsoid(const Parameters& par)
                    : base_t_fi<base_eqdc_ellipsoid<CalculationType, Parameters>,
                     CalculationType, Parameters>(*this, par) {}

                // FORWARD(e_forward)  sphere & ellipsoid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double rho = 0.0;
                    rho = this->m_proj_parm.c - (this->m_proj_parm.ellips ? pj_mlfn(lp_lat, sin(lp_lat),
                        cos(lp_lat), this->m_proj_parm.en) : lp_lat);
                    xy_x = rho * sin( lp_lon *= this->m_proj_parm.n );
                    xy_y = this->m_proj_parm.rho0 - rho * cos(lp_lon);
                }

                // INVERSE(e_inverse)  sphere & ellipsoid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double rho = 0.0;
                    if ((rho = boost::math::hypot(xy_x, xy_y = this->m_proj_parm.rho0 - xy_y)) != 0.0 ) {
                        if (this->m_proj_parm.n < 0.) {
                            rho = -rho;
                            xy_x = -xy_x;
                            xy_y = -xy_y;
                        }
                        lp_lat = this->m_proj_parm.c - rho;
                        if (this->m_proj_parm.ellips)
                            lp_lat = pj_inv_mlfn(lp_lat, this->m_par.es, this->m_proj_parm.en);
                        lp_lon = atan2(xy_x, xy_y) / this->m_proj_parm.n;
                    } else {
                        lp_lon = 0.;
                        lp_lat = this->m_proj_parm.n > 0. ? geometry::math::half_pi<double>() : - geometry::math::half_pi<double>();
                    }
                }

                // SPECIAL(fac)
                #ifdef SPECIAL_FACTORS_NOT_CONVERTED
                inline void fac(Geographic lp, Factors &fac) const
                {
                    double sinphi, cosphi;

                    sinphi = sin(lp_lat);
                    cosphi = cos(lp_lat);
                    this->m_fac.code |= IS_ANAL_HK;
                    this->m_fac.h = 1.;
                    this->m_fac.k = this->m_proj_parm.n * (this->m_proj_parm.c - (this->m_proj_parm.ellips ? pj_mlfn(lp_lat, sinphi,
                        cosphi, this->m_proj_parm.en) : lp_lat)) / pj_msfn(sinphi, cosphi, this->m_par.es);
                }
                #endif

                static inline std::string get_name()
                {
                    return "eqdc_ellipsoid";
                }

            };

            // Equidistant Conic
            template <typename Parameters>
            void setup_eqdc(Parameters& par, par_eqdc& proj_parm)
            {
                double cosphi, sinphi;
                int secant;

                proj_parm.phi1 = pj_param(par.params, "rlat_1").f;
                proj_parm.phi2 = pj_param(par.params, "rlat_2").f;
                if (fabs(proj_parm.phi1 + proj_parm.phi2) < EPS10) throw proj_exception(-21);
                if (!pj_enfn(par.es, proj_parm.en))
                    throw proj_exception(0);
                proj_parm.n = sinphi = sin(proj_parm.phi1);
                cosphi = cos(proj_parm.phi1);
                secant = fabs(proj_parm.phi1 - proj_parm.phi2) >= EPS10;
                if( (proj_parm.ellips = (par.es > 0.)) ) {
                    double ml1, m1;

                    m1 = pj_msfn(sinphi, cosphi, par.es);
                    ml1 = pj_mlfn(proj_parm.phi1, sinphi, cosphi, proj_parm.en);
                    if (secant) { /* secant cone */
                        sinphi = sin(proj_parm.phi2);
                        cosphi = cos(proj_parm.phi2);
                        proj_parm.n = (m1 - pj_msfn(sinphi, cosphi, par.es)) /
                            (pj_mlfn(proj_parm.phi2, sinphi, cosphi, proj_parm.en) - ml1);
                    }
                    proj_parm.c = ml1 + m1 / proj_parm.n;
                    proj_parm.rho0 = proj_parm.c - pj_mlfn(par.phi0, sin(par.phi0),
                        cos(par.phi0), proj_parm.en);
                } else {
                    if (secant)
                        proj_parm.n = (cosphi - cos(proj_parm.phi2)) / (proj_parm.phi2 - proj_parm.phi1);
                    proj_parm.c = proj_parm.phi1 + cos(proj_parm.phi1) / proj_parm.n;
                    proj_parm.rho0 = proj_parm.c - par.phi0;
                }
            }

    }} // namespace detail::eqdc
    #endif // doxygen

    /*!
        \brief Equidistant Conic projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
            - Conic
            - Spheroid
            - Ellipsoid
        \par Projection parameters
            - lat_1: Latitude of first standard parallel (degrees)
            - lat_2: Latitude of second standard parallel (degrees)
        \par Example
        \image html ex_eqdc.gif
    */
    template <typename CalculationType, typename Parameters = parameters>
    struct eqdc_ellipsoid : public detail::eqdc::base_eqdc_ellipsoid<CalculationType, Parameters>
    {
        inline eqdc_ellipsoid(const Parameters& par) : detail::eqdc::base_eqdc_ellipsoid<CalculationType, Parameters>(par)
        {
            detail::eqdc::setup_eqdc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Static projection
        BOOST_GEOMETRY_PROJECTIONS_DETAIL_STATIC_PROJECTION(srs::proj::eqdc, eqdc_ellipsoid, eqdc_ellipsoid)

        // Factory entry(s)
        template <typename CalculationType, typename Parameters>
        class eqdc_entry : public detail::factory_entry<CalculationType, Parameters>
        {
            public :
                virtual base_v<CalculationType, Parameters>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<eqdc_ellipsoid<CalculationType, Parameters>, CalculationType, Parameters>(par);
                }
        };

        template <typename CalculationType, typename Parameters>
        inline void eqdc_init(detail::base_factory<CalculationType, Parameters>& factory)
        {
            factory.add_to_factory("eqdc", new eqdc_entry<CalculationType, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

} // namespace projections

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_PROJECTIONS_EQDC_HPP

