// Boost.Geometry

// Copyright (c) 2018 Adeel Ahmad, Islamabad, Pakistan.

// Contributed and/or modified by Adeel Ahmad, as part of Google Summer of Code 2018 program.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from GeographicLib, https://geographiclib.sourceforge.io
// GeographicLib is originally written by Charles Karney.

// Author: Charles Karney (2008-2017)

// Last updated version of GeographicLib: 1.49

// Original copyright notice:

// Copyright (c) Charles Karney (2008-2017) <charles@karney.com> and licensed
// under the MIT/X11 License. For more information, see
// https://geographiclib.sourceforge.io

#ifndef BOOST_GEOMETRY_UTIL_SERIES_EXPANSION_HPP
#define BOOST_GEOMETRY_UTIL_SERIES_EXPANSION_HPP

#include <boost/geometry/core/assert.hpp>
#include <boost/geometry/util/math.hpp>

namespace boost { namespace geometry { namespace series_expansion {

    /*
     Generate and evaluate the series expansion of the following integral

     I1 = integrate( sqrt(1+k2*sin(sigma1)^2), sigma1, 0, sigma )

     which is valid for k2 small. We substitute k2 = 4 * eps / (1 - eps)^2
     and expand (1 - eps) * I1 retaining terms up to order eps^maxpow
     in A1 and C1[l].

     The resulting series is of the form

     A1 * ( sigma + sum(C1[l] * sin(2*l*sigma), l, 1, maxpow) ).

     The scale factor A1-1 = mean value of (d/dsigma)I1 - 1

     The expansion above is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_A1 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac

     To replace each number x by CT(x) the following
     script can be used:
       sed -e 's/[0-9]\+/CT(&)/g; s/\[CT/\[/g; s/)\]/\]/g;
               s/case\sCT(/case /g; s/):/:/g; s/epsCT(2)/eps2/g;'
    */
    template <size_t SeriesOrder, typename CT>
    inline CT evaluate_A1(CT eps)
    {
        CT eps2 = math::sqr(eps);
        CT t;
        switch (SeriesOrder/2) {
        case 0:
            t = CT(0);
            break;
        case 1:
            t = eps2/CT(4);
            break;
        case 2:
            t = eps2*(eps2+CT(16))/CT(64);
            break;
        case 3:
            t = eps2*(eps2*(eps2+CT(4))+CT(64))/CT(256);
            break;
        case 4:
            t = eps2*(eps2*(eps2*(CT(25)*eps2+CT(64))+CT(256))+CT(4096))/CT(16384);
            break;
        }
        return (t + eps) / (CT(1) - eps);
    }

    /*
     Generate and evaluate the series expansion of the following integral

     I2 = integrate( 1/sqrt(1+k2*sin(sigma1)^2), sigma1, 0, sigma )

     which is valid for k2 small. We substitute k2 = 4 * eps / (1 - eps)^2
     and expand (1 - eps) * I2 retaining terms up to order eps^maxpow
     in A2 and C2[l].

     The resulting series is of the form

     A2 * ( sigma + sum(C2[l] * sin(2*l*sigma), l, 1, maxpow) )

     The scale factor A2-1 = mean value of (d/dsigma)2 - 1

     The expansion above is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_A2 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <size_t SeriesOrder, typename CT>
    inline CT evaluate_A2(CT const& eps)
    {
        CT const eps2 = math::sqr(eps);
        CT t;
        switch (SeriesOrder/2) {
        case 0:
            t = CT(0);
            break;
        case 1:
            t = -CT(3)*eps2/CT(4);
            break;
        case 2:
            t = (-CT(7)*eps2-CT(48))*eps2/CT(64);
            break;
        case 3:
            t = eps2*((-CT(11)*eps2-CT(28))*eps2-CT(192))/CT(256);
            break;
        default:
            t = eps2*(eps2*((-CT(375)*eps2-CT(704))*eps2-CT(1792))-CT(12288))/CT(16384);
            break;
        }
        return (t - eps) / (CT(1) + eps);
    }

    /*
     Express

        I3 = integrate( (2-f)/(1+(1-f)*sqrt(1+k2*sin(sigma1)^2)), sigma1, 0, sigma )

     as a series

        A3 * ( sigma + sum(C3[l] * sin(2*l*sigma), l, 1, maxpow-1) )

     valid for f and k2 small.  It is convenient to write k2 = 4 * eps / (1 -
     eps)^2 and f = 2*n/(1+n) and expand in eps and n.  This procedure leads
     to a series where the coefficients of eps^j are terminating series in n.

     The scale factor A3 = mean value of (d/dsigma)I3

     The expansion above is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_coeffs_A3 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <typename Coeffs, typename CT>
    inline void evaluate_coeffs_A3(Coeffs &c, CT const& n)
    {
        switch (int(Coeffs::static_size)) {
        case 0:
            break;
        case 1:
            c[0] = CT(1);
            break;
        case 2:
            c[0] = CT(1);
            c[1] = -CT(1)/CT(2);
            break;
        case 3:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = -CT(1)/CT(4);
            break;
        case 4:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = (-n-CT(2))/CT(8);
            c[3] = -CT(1)/CT(16);
            break;
        case 5:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = (n*(CT(3)*n-CT(1))-CT(2))/CT(8);
            c[3] = (-CT(3)*n-CT(1))/CT(16);
            c[4] = -CT(3)/CT(64);
            break;
        case 6:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = (n*(CT(3)*n-CT(1))-CT(2))/CT(8);
            c[3] = ((-n-CT(3))*n-CT(1))/CT(16);
            c[4] = (-CT(2)*n-CT(3))/CT(64);
            c[5] = -CT(3)/CT(128);
            break;
        case 7:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = (n*(CT(3)*n-CT(1))-CT(2))/CT(8);
            c[3] = (n*(n*(CT(5)*n-CT(1))-CT(3))-CT(1))/CT(16);
            c[4] = ((-CT(10)*n-CT(2))*n-CT(3))/CT(64);
            c[5] = (-CT(5)*n-CT(3))/CT(128);
            c[6] = -CT(5)/CT(256);
            break;
        default:
            c[0] = CT(1);
            c[1] = (n-CT(1))/CT(2);
            c[2] = (n*(CT(3)*n-CT(1))-CT(2))/CT(8);
            c[3] = (n*(n*(CT(5)*n-CT(1))-CT(3))-CT(1))/CT(16);
            c[4] = (n*((-CT(5)*n-CT(20))*n-CT(4))-CT(6))/CT(128);
            c[5] = ((-CT(5)*n-CT(10))*n-CT(6))/CT(256);
            c[6] = (-CT(15)*n-CT(20))/CT(1024);
            c[7] = -CT(25)/CT(2048);
            break;
        }
    }

    /*
     The coefficients C1[l] in the Fourier expansion of B1.

     The expansion below is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_coeffs_C1 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <typename Coeffs, typename CT>
    inline void evaluate_coeffs_C1(Coeffs &c, CT const& eps)
    {
        CT eps2 = math::sqr(eps);
        CT d = eps;
        switch (int(Coeffs::static_size) - 1) {
        case 0:
            break;
        case 1:
            c[1] = -d/CT(2);
            break;
        case 2:
            c[1] = -d/CT(2);
            d *= eps;
            c[2] = -d/CT(16);
            break;
        case 3:
            c[1] = d*(CT(3)*eps2-CT(8))/CT(16);
            d *= eps;
            c[2] = -d/CT(16);
            d *= eps;
            c[3] = -d/CT(48);
            break;
        case 4:
            c[1] = d*(CT(3)*eps2-CT(8))/CT(16);
            d *= eps;
            c[2] = d*(eps2-CT(2))/CT(32);
            d *= eps;
            c[3] = -d/CT(48);
            d *= eps;
            c[4] = -CT(5)*d/CT(512);
            break;
        case 5:
            c[1] = d*((CT(6)-eps2)*eps2-CT(16))/CT(32);
            d *= eps;
            c[2] = d*(eps2-CT(2))/CT(32);
            d *= eps;
            c[3] = d*(CT(9)*eps2-CT(16))/CT(768);
            d *= eps;
            c[4] = -CT(5)*d/CT(512);
            d *= eps;
            c[5] = -CT(7)*d/CT(1280);
            break;
        case 6:
            c[1] = d*((CT(6)-eps2)*eps2-CT(16))/CT(32);
            d *= eps;
            c[2] = d*((CT(64)-CT(9)*eps2)*eps2-CT(128))/CT(2048);
            d *= eps;
            c[3] = d*(CT(9)*eps2-CT(16))/CT(768);
            d *= eps;
            c[4] = d*(CT(3)*eps2-CT(5))/CT(512);
            d *= eps;
            c[5] = -CT(7)*d/CT(1280);
            d *= eps;
            c[6] = -CT(7)*d/CT(2048);
            break;
        case 7:
            c[1] = d*(eps2*(eps2*(CT(19)*eps2-CT(64))+CT(384))-CT(1024))/CT(2048);
            d *= eps;
            c[2] = d*((CT(64)-CT(9)*eps2)*eps2-CT(128))/CT(2048);
            d *= eps;
            c[3] = d*((CT(72)-CT(9)*eps2)*eps2-CT(128))/CT(6144);
            d *= eps;
            c[4] = d*(CT(3)*eps2-CT(5))/CT(512);
            d *= eps;
            c[5] = d*(CT(35)*eps2-CT(56))/CT(10240);
            d *= eps;
            c[6] = -CT(7)*d/CT(2048);
            d *= eps;
            c[7] = -CT(33)*d/CT(14336);
            break;
        default:
            c[1] = d*(eps2*(eps2*(CT(19)*eps2-CT(64))+CT(384))-CT(1024))/CT(2048);
            d *= eps;
            c[2] = d*(eps2*(eps2*(CT(7)*eps2-CT(18))+CT(128))-CT(256))/CT(4096);
            d *= eps;
            c[3] = d*((CT(72)-CT(9)*eps2)*eps2-CT(128))/CT(6144);
            d *= eps;
            c[4] = d*((CT(96)-CT(11)*eps2)*eps2-CT(160))/CT(16384);
            d *= eps;
            c[5] = d*(CT(35)*eps2-CT(56))/CT(10240);
            d *= eps;
            c[6] = d*(CT(9)*eps2-CT(14))/CT(4096);
            d *= eps;
            c[7] = -CT(33)*d/CT(14336);
            d *= eps;
            c[8] = -CT(429)*d/CT(262144);
            break;
        }
    }

    /*
     The coefficients C1p[l] in the Fourier expansion of B1p.

     The expansion below is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_coeffs_C1p below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <typename Coeffs, typename CT>
    inline void evaluate_coeffs_C1p(Coeffs& c, CT const& eps)
    {
        CT const eps2 = math::sqr(eps);
        CT d = eps;
        switch (int(Coeffs::static_size) - 1) {
        case 0:
            break;
        case 1:
            c[1] = d/CT(2);
            break;
        case 2:
            c[1] = d/CT(2);
            d *= eps;
            c[2] = CT(5)*d/CT(16);
            break;
        case 3:
            c[1] = d*(CT(16)-CT(9)*eps2)/CT(32);
            d *= eps;
            c[2] = CT(5)*d/CT(16);
            d *= eps;
            c[3] = CT(29)*d/CT(96);
            break;
        case 4:
            c[1] = d*(CT(16)-CT(9)*eps2)/CT(32);
            d *= eps;
            c[2] = d*(CT(30)-CT(37)*eps2)/CT(96);
            d *= eps;
            c[3] = CT(29)*d/CT(96);
            d *= eps;
            c[4] = CT(539)*d/CT(1536);
            break;
        case 5:
            c[1] = d*(eps2*(CT(205)*eps2-CT(432))+CT(768))/CT(1536);
            d *= eps;
            c[2] = d*(CT(30)-CT(37)*eps2)/CT(96);
            d *= eps;
            c[3] = d*(CT(116)-CT(225)*eps2)/CT(384);
            d *= eps;
            c[4] = CT(539)*d/CT(1536);
            d *= eps;
            c[5] = CT(3467)*d/CT(7680);
            break;
        case 6:
            c[1] = d*(eps2*(CT(205)*eps2-CT(432))+CT(768))/CT(1536);
            d *= eps;
            c[2] = d*(eps2*(CT(4005)*eps2-CT(4736))+CT(3840))/CT(12288);
            d *= eps;
            c[3] = d*(CT(116)-CT(225)*eps2)/CT(384);
            d *= eps;
            c[4] = d*(CT(2695)-CT(7173)*eps2)/CT(7680);
            d *= eps;
            c[5] = CT(3467)*d/CT(7680);
            d *= eps;
            c[6] = CT(38081)*d/CT(61440);
            break;
        case 7:
            c[1] = d*(eps2*((CT(9840)-CT(4879)*eps2)*eps2-CT(20736))+CT(36864))/CT(73728);
            d *= eps;
            c[2] = d*(eps2*(CT(4005)*eps2-CT(4736))+CT(3840))/CT(12288);
            d *= eps;
            c[3] = d*(eps2*(CT(8703)*eps2-CT(7200))+CT(3712))/CT(12288);
            d *= eps;
            c[4] = d*(CT(2695)-CT(7173)*eps2)/CT(7680);
            d *= eps;
            c[5] = d*(CT(41604)-CT(141115)*eps2)/CT(92160);
            d *= eps;
            c[6] = CT(38081)*d/CT(61440);
            d *= eps;
            c[7] = CT(459485)*d/CT(516096);
            break;
        default:
            c[1] = d*(eps2*((CT(9840)-CT(4879)*eps2)*eps2-CT(20736))+CT(36864))/CT(73728);
            d *= eps;
            c[2] = d*(eps2*((CT(120150)-CT(86171)*eps2)*eps2-CT(142080))+CT(115200))/CT(368640);
            d *= eps;
            c[3] = d*(eps2*(CT(8703)*eps2-CT(7200))+CT(3712))/CT(12288);
            d *= eps;
            c[4] = d*(eps2*(CT(1082857)*eps2-CT(688608))+CT(258720))/CT(737280);
            d *= eps;
            c[5] = d*(CT(41604)-CT(141115)*eps2)/CT(92160);
            d *= eps;
            c[6] = d*(CT(533134)-CT(2200311)*eps2)/CT(860160);
            d *= eps;
            c[7] = CT(459485)*d/CT(516096);
            d *= eps;
            c[8] = CT(109167851)*d/CT(82575360);
            break;
        }
    }

    /*
     The coefficients C2[l] in the Fourier expansion of B2.

     The expansion below is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_coeffs_C2 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <typename Coeffs, typename CT>
    inline void evaluate_coeffs_C2(Coeffs& c, CT const& eps)
    {
        CT const eps2 = math::sqr(eps);
        CT d = eps;
        switch (int(Coeffs::static_size) - 1) {
        case 0:
            break;
        case 1:
            c[1] = d/CT(2);
            break;
        case 2:
            c[1] = d/CT(2);
            d *= eps;
            c[2] = CT(3)*d/CT(16);
            break;
        case 3:
            c[1] = d*(eps2+CT(8))/CT(16);
            d *= eps;
            c[2] = CT(3)*d/CT(16);
            d *= eps;
            c[3] = CT(5)*d/CT(48);
            break;
        case 4:
            c[1] = d*(eps2+CT(8))/CT(16);
            d *= eps;
            c[2] = d*(eps2+CT(6))/CT(32);
            d *= eps;
            c[3] = CT(5)*d/CT(48);
            d *= eps;
            c[4] = CT(35)*d/CT(512);
            break;
        case 5:
            c[1] = d*(eps2*(eps2+CT(2))+CT(16))/CT(32);
            d *= eps;
            c[2] = d*(eps2+CT(6))/CT(32);
            d *= eps;
            c[3] = d*(CT(15)*eps2+CT(80))/CT(768);
            d *= eps;
            c[4] = CT(35)*d/CT(512);
            d *= eps;
            c[5] = CT(63)*d/CT(1280);
            break;
        case 6:
            c[1] = d*(eps2*(eps2+CT(2))+CT(16))/CT(32);
            d *= eps;
            c[2] = d*(eps2*(CT(35)*eps2+CT(64))+CT(384))/CT(2048);
            d *= eps;
            c[3] = d*(CT(15)*eps2+CT(80))/CT(768);
            d *= eps;
            c[4] = d*(CT(7)*eps2+CT(35))/CT(512);
            d *= eps;
            c[5] = CT(63)*d/CT(1280);
            d *= eps;
            c[6] = CT(77)*d/CT(2048);
            break;
        case 7:
            c[1] = d*(eps2*(eps2*(CT(41)*eps2+CT(64))+CT(128))+CT(1024))/CT(2048);
            d *= eps;
            c[2] = d*(eps2*(CT(35)*eps2+CT(64))+CT(384))/CT(2048);
            d *= eps;
            c[3] = d*(eps2*(CT(69)*eps2+CT(120))+CT(640))/CT(6144);
            d *= eps;
            c[4] = d*(CT(7)*eps2+CT(35))/CT(512);
            d *= eps;
            c[5] = d*(CT(105)*eps2+CT(504))/CT(10240);
            d *= eps;
            c[6] = CT(77)*d/CT(2048);
            d *= eps;
            c[7] = CT(429)*d/CT(14336);
            break;
        default:
            c[1] = d*(eps2*(eps2*(CT(41)*eps2+CT(64))+CT(128))+CT(1024))/CT(2048);
            d *= eps;
            c[2] = d*(eps2*(eps2*(CT(47)*eps2+CT(70))+CT(128))+CT(768))/CT(4096);
            d *= eps;
            c[3] = d*(eps2*(CT(69)*eps2+CT(120))+CT(640))/CT(6144);
            d *= eps;
            c[4] = d*(eps2*(CT(133)*eps2+CT(224))+CT(1120))/CT(16384);
            d *= eps;
            c[5] = d*(CT(105)*eps2+CT(504))/CT(10240);
            d *= eps;
            c[6] = d*(CT(33)*eps2+CT(154))/CT(4096);
            d *= eps;
            c[7] = CT(429)*d/CT(14336);
            d *= eps;
            c[8] = CT(6435)*d/CT(262144);
            break;
        }
    }

    /*
     The coefficients C3[l] in the Fourier expansion of B3.

     The expansion below is performed in Maxima, a Computer Algebra System.
     The C++ code (that yields the function evaluate_coeffs_C3 below) is
     generated by the following Maxima script:
     geometry/doc/other/maxima/geod.mac
    */
    template <size_t SeriesOrder, typename Coeffs, typename CT>
    inline void evaluate_coeffs_C3x(Coeffs &c, CT const& n) {
        size_t const coeff_size = Coeffs::static_size;
        size_t const expected_size = (SeriesOrder * (SeriesOrder - 1)) / 2;
        BOOST_GEOMETRY_ASSERT((coeff_size == expected_size));

        const CT n2 = math::sqr(n);
        switch (SeriesOrder) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            c[0] = (CT(1)-n)/CT(4);
            break;
        case 3:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = ((n-CT(3))*n+CT(2))/CT(32);
            break;
        case 4:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = (n*((-CT(5)*n-CT(1))*n+CT(3))+CT(3))/CT(64);
            c[3] = ((n-CT(3))*n+CT(2))/CT(32);
            c[4] = (n*(n*(CT(2)*n-CT(3))-CT(2))+CT(3))/CT(64);
            c[5] = (n*((CT(5)-n)*n-CT(9))+CT(5))/CT(192);
            break;
        case 5:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = (n*((-CT(5)*n-CT(1))*n+CT(3))+CT(3))/CT(64);
            c[3] = (n*((CT(2)-CT(2)*n)*n+CT(2))+CT(5))/CT(128);
            c[4] = ((n-CT(3))*n+CT(2))/CT(32);
            c[5] = (n*(n*(CT(2)*n-CT(3))-CT(2))+CT(3))/CT(64);
            c[6] = (n*((-CT(6)*n-CT(9))*n+CT(2))+CT(6))/CT(256);
            c[7] = (n*((CT(5)-n)*n-CT(9))+CT(5))/CT(192);
            c[8] = (n*(n*(CT(10)*n-CT(6))-CT(10))+CT(9))/CT(384);
            c[9] = (n*((CT(20)-CT(7)*n)*n-CT(28))+CT(14))/CT(1024);
            break;
        case 6:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = (n*((-CT(5)*n-CT(1))*n+CT(3))+CT(3))/CT(64);
            c[3] = (n*((CT(2)-CT(2)*n)*n+CT(2))+CT(5))/CT(128);
            c[4] = (n*(CT(3)*n+CT(11))+CT(12))/CT(512);
            c[5] = ((n-CT(3))*n+CT(2))/CT(32);
            c[6] = (n*(n*(CT(2)*n-CT(3))-CT(2))+CT(3))/CT(64);
            c[7] = (n*((-CT(6)*n-CT(9))*n+CT(2))+CT(6))/CT(256);
            c[8] = ((CT(1)-CT(2)*n)*n+CT(5))/CT(256);
            c[9] = (n*((CT(5)-n)*n-CT(9))+CT(5))/CT(192);
            c[10] = (n*(n*(CT(10)*n-CT(6))-CT(10))+CT(9))/CT(384);
            c[11] = ((-CT(77)*n-CT(8))*n+CT(42))/CT(3072);
            c[12] = (n*((CT(20)-CT(7)*n)*n-CT(28))+CT(14))/CT(1024);
            c[13] = ((-CT(7)*n-CT(40))*n+CT(28))/CT(2048);
            c[14] = (n*(CT(75)*n-CT(90))+CT(42))/CT(5120);
            break;
        case 7:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = (n*((-CT(5)*n-CT(1))*n+CT(3))+CT(3))/CT(64);
            c[3] = (n*((CT(2)-CT(2)*n)*n+CT(2))+CT(5))/CT(128);
            c[4] = (n*(CT(3)*n+CT(11))+CT(12))/CT(512);
            c[5] = (CT(10)*n+CT(21))/CT(1024);
            c[6] = ((n-CT(3))*n+CT(2))/CT(32);
            c[7] = (n*(n*(CT(2)*n-CT(3))-CT(2))+CT(3))/CT(64);
            c[8] = (n*((-CT(6)*n-CT(9))*n+CT(2))+CT(6))/CT(256);
            c[9] = ((CT(1)-CT(2)*n)*n+CT(5))/CT(256);
            c[10] = (CT(69)*n+CT(108))/CT(8192);
            c[11] = (n*((CT(5)-n)*n-CT(9))+CT(5))/CT(192);
            c[12] = (n*(n*(CT(10)*n-CT(6))-CT(10))+CT(9))/CT(384);
            c[13] = ((-CT(77)*n-CT(8))*n+CT(42))/CT(3072);
            c[14] = (CT(12)-n)/CT(1024);
            c[15] = (n*((CT(20)-CT(7)*n)*n-CT(28))+CT(14))/CT(1024);
            c[16] = ((-CT(7)*n-CT(40))*n+CT(28))/CT(2048);
            c[17] = (CT(72)-CT(43)*n)/CT(8192);
            c[18] = (n*(CT(75)*n-CT(90))+CT(42))/CT(5120);
            c[19] = (CT(9)-CT(15)*n)/CT(1024);
            c[20] = (CT(44)-CT(99)*n)/CT(8192);
            break;
        default:
            c[0] = (CT(1)-n)/CT(4);
            c[1] = (CT(1)-n2)/CT(8);
            c[2] = (n*((-CT(5)*n-CT(1))*n+CT(3))+CT(3))/CT(64);
            c[3] = (n*((CT(2)-CT(2)*n)*n+CT(2))+CT(5))/CT(128);
            c[4] = (n*(CT(3)*n+CT(11))+CT(12))/CT(512);
            c[5] = (CT(10)*n+CT(21))/CT(1024);
            c[6] = CT(243)/CT(16384);
            c[7] = ((n-CT(3))*n+CT(2))/CT(32);
            c[8] = (n*(n*(CT(2)*n-CT(3))-CT(2))+CT(3))/CT(64);
            c[9] = (n*((-CT(6)*n-CT(9))*n+CT(2))+CT(6))/CT(256);
            c[10] = ((CT(1)-CT(2)*n)*n+CT(5))/CT(256);
            c[11] = (CT(69)*n+CT(108))/CT(8192);
            c[12] = CT(187)/CT(16384);
            c[13] = (n*((CT(5)-n)*n-CT(9))+CT(5))/CT(192);
            c[14] = (n*(n*(CT(10)*n-CT(6))-CT(10))+CT(9))/CT(384);
            c[15] = ((-CT(77)*n-CT(8))*n+CT(42))/CT(3072);
            c[16] = (CT(12)-n)/CT(1024);
            c[17] = CT(139)/CT(16384);
            c[18] = (n*((CT(20)-CT(7)*n)*n-CT(28))+CT(14))/CT(1024);
            c[19] = ((-CT(7)*n-CT(40))*n+CT(28))/CT(2048);
            c[20] = (CT(72)-CT(43)*n)/CT(8192);
            c[21] = CT(127)/CT(16384);
            c[22] = (n*(CT(75)*n-CT(90))+CT(42))/CT(5120);
            c[23] = (CT(9)-CT(15)*n)/CT(1024);
            c[24] = CT(99)/CT(16384);
            c[25] = (CT(44)-CT(99)*n)/CT(8192);
            c[26] = CT(99)/CT(16384);
            c[27] = CT(429)/CT(114688);
            break;
        }
    }

    /*
    \brief Given the set of coefficients coeffs2[] evaluate on
      C3 and return the set of coefficients coeffs1[].

      Elements coeffs1[1] through coeffs1[SeriesOrder - 1] are set.
    */
    template <typename Coeffs1, typename Coeffs2, typename CT>
    inline void evaluate_coeffs_C3(Coeffs1 &coeffs1, Coeffs2 &coeffs2, CT const& eps)
    {
        CT mult = 1;
        size_t offset = 0;

        // i is the index of C3[i].
        for (size_t i = 1; i < Coeffs1::static_size; ++i)
        {
            // Order of polynomial in eps.
            size_t m = Coeffs1::static_size - i;
            mult *= eps;

            coeffs1[i] = mult * math::horner_evaluate(eps, coeffs2.begin() + offset,
                                                           coeffs2.begin() + offset + m);

            offset += m;
        }
        // Post condition: offset == coeffs_C3_size
    }

    /*
    \brief Evaluate the following:

     y = sum(c[i] * sin(2*i * x), i, 1, n)

     using Clenshaw summation.
    */
    template <typename CT, typename Coeffs>
    inline CT sin_cos_series(CT const& sinx, CT const& cosx, Coeffs const& coeffs)
    {
        size_t n = Coeffs::static_size - 1;
        size_t index = 0;

        // Point to one beyond last element.
        index += (n + 1);
        CT ar = 2 * (cosx - sinx) * (cosx + sinx);

        // If n is odd, get the last element.
        CT k0 = n & 1 ? coeffs[--index] : 0;
        CT k1 = 0;

        // Make n even.
        n /= 2;
        while (n--) {
            // Unroll loop x 2, so accumulators return to their original role.
            k1 = ar * k0 - k1 + coeffs[--index];
            k0 = ar * k1 - k0 + coeffs[--index];
        }

        return 2 * sinx * cosx * k0;
    }

    /*
     The coefficient containers for the series expansions.
     These structs allow the caller to only know the series order.
    */
    template <size_t SeriesOrder, typename CT>
    struct coeffs_C1 : boost::array<CT, SeriesOrder + 1>
    {
        coeffs_C1(CT const& epsilon)
        {
            evaluate_coeffs_C1(*this, epsilon);
        }
    };

    template <size_t SeriesOrder, typename CT>
    struct coeffs_C1p : boost::array<CT, SeriesOrder + 1>
    {
        coeffs_C1p(CT const& epsilon)
        {
            evaluate_coeffs_C1p(*this, epsilon);
        }
    };

    template <size_t SeriesOrder, typename CT>
    struct coeffs_C2 : boost::array<CT, SeriesOrder + 1>
    {
        coeffs_C2(CT const& epsilon)
        {
            evaluate_coeffs_C2(*this, epsilon);
        }
    };

    template <size_t SeriesOrder, typename CT>
    struct coeffs_C3x : boost::array<CT, (SeriesOrder * (SeriesOrder - 1)) / 2>
    {
        coeffs_C3x(CT const& n)
        {
            evaluate_coeffs_C3x<SeriesOrder>(*this, n);
        }
    };

    template <size_t SeriesOrder, typename CT>
    struct coeffs_C3 : boost::array<CT, SeriesOrder>
    {
        coeffs_C3(CT const& n, CT const& epsilon)
        {
            coeffs_C3x<SeriesOrder, CT> coeffs_C3x(n);

            evaluate_coeffs_C3(*this, coeffs_C3x, epsilon);
        }
    };

    template <size_t SeriesOrder, typename CT>
    struct coeffs_A3 : boost::array<CT, SeriesOrder>
    {
        coeffs_A3(CT const& n)
        {
            evaluate_coeffs_A3(*this, n);
        }
    };

}}} // namespace boost::geometry::series_expansion

#endif // BOOST_GEOMETRY_UTIL_SERIES_EXPANSION_HPP
