// ==========================================================================
// $Source: /var/lib/cvs/Givaro/src/kernel/gmp++/gmp++_int_gcd.C,v $
// Copyright(c)'1994-2009 by The Givaro group
// This file is part of Givaro.
// Givaro is governed by the CeCILL-B license under French law
// and abiding by the rules of distribution of free software.
// see the COPYRIGHT file for more details.
// Authors: M. Samama, T. Gautier
// $Id: gmp++_int_gcd.C,v 1.4 2011-01-18 17:49:06 jgdumas Exp $
// ==========================================================================
// Description:
//
/** @file gmp++/gmp++_int_gcd.C
 * gcding stuff.
 */

#ifndef __GIVARO_gmpxx_gmpxx_int_gcd_C
#define __GIVARO_gmpxx_gmpxx_int_gcd_C

#ifndef __GIVARO_INLINE_ALL
#include "gmp++/gmp++.h"
#endif
#include "givaro/giverror.h"
namespace Givaro {
    // ==========================================================================
    // Computes and returns the lcm of the two integers a and b.
    Integer lcm(const Integer& a, const Integer& b) {
        Integer Res(Integer::one);
        mpz_lcm( (mpz_ptr)&(Res.gmp_rep), (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (Res.priv_sign() <0) return -Res;
        else return Res ;
    }

    Integer& lcm(Integer& g, const Integer& a, const Integer& b) {
        mpz_lcm( (mpz_ptr)&(g.gmp_rep), (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (g.priv_sign() <0) return Integer::negin(g);
        else return g ;
    }


    // ==========================================================================
    // Computes and returns the gcd of the two integers a and b.
    Integer gcd(const Integer& a, const Integer& b)
    {
        Integer Res(Integer::one);
        mpz_gcd( (mpz_ptr)&(Res.gmp_rep), (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (Res.priv_sign() <0) return -Res;
        return Res ;
    }

    Integer& gcd(Integer& g, const Integer& a, const Integer& b)
    {
        mpz_gcd( (mpz_ptr)&(g.gmp_rep), (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (g.priv_sign() <0) return Integer::negin(g);
        return g ;
    }

    Integer& inv(Integer& u, const Integer& a, const Integer& b) {
#ifdef GIVARO_DEBUG
        const int res =
#endif
        mpz_invert( (mpz_ptr)&(u.gmp_rep), (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
#ifdef GIVARO_DEBUG
        if(! res) {
            throw GivMathDivZero("*** Error: division by zero, in operator Integer::inv in gmp++_int_gcd.C") ;
        }
#endif
        return u ;
    }

    Integer& invin(Integer& u, const Integer& b) {
        return inv(u,u,b);
    }

    // ==========================================================================
    // Computes and returns the gcd g of the two integers a and b such that
    // g = a*u + b*v .
    // The algorithm used is this of Gmp.
    Integer  gcd (Integer& u, Integer& v,
                  const Integer& a, const Integer& b )
    {
        v = 1; // v must not be 0 to be computed.
        Integer Res(Integer::one);
        mpz_gcdext( (mpz_ptr)&(Res.gmp_rep), (mpz_ptr)&(u.gmp_rep), (mpz_ptr)&(v.gmp_rep),
                    (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (Res.priv_sign() < 0) {
            Integer::negin(u);
            Integer::negin(v);
            return Integer::negin(Res);
        }
        //   { u = -u ; v = -v ; return -Res;}
        return Res;
    }

    Integer&  gcd (Integer& g, Integer& u, Integer& v,
                   const Integer& a, const Integer& b)
    {
        v = 1; // v must not be 0 to be computed.
        mpz_gcdext( (mpz_ptr)&(g.gmp_rep), (mpz_ptr)&(u.gmp_rep), (mpz_ptr)&(v.gmp_rep),
                    (mpz_srcptr)&(a.gmp_rep), (mpz_srcptr)&(b.gmp_rep) ) ;
        if (g.priv_sign() < 0) { Integer::negin(u); Integer::negin(v); return Integer::negin(g);}
        return g;
    }


    Integer pp( const Integer& P, const Integer& Q )
    {
        Integer U = P ;
        Integer V = gcd(P,Q) ;
        // -- computes the prime part U of g relatively to U
        while ( V != Integer::one )
        {
            U = U / V ;
            V = gcd( U,V) ;
        }
        return U ;
    }

}

#endif // __GIVARO_gmpxx_gmpxx_int_gcd_C

/* -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
// vim:sts=4:sw=4:ts=4:et:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
