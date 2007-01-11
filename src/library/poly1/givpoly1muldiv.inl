// ==========================================================================
// $Source: /var/lib/cvs/Givaro/src/library/poly1/givpoly1muldiv.inl,v $
// Copyright(c)'94-97 by Givaro Team
// see the copyright file.
// Authors: T. Gautier
// $Id: givpoly1muldiv.inl,v 1.6 2007-01-11 18:42:51 jgdumas Exp $
// ==========================================================================
#include "givaro/givpower.h"
#include "givaro/giverror.h"


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::shiftin ( Rep& R, int s) const 
{
    Type_t zer;
    R.insert(R.begin(), s, this->_domain.init(zer,0) );
    return R;
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::shift ( Rep& R, const Rep& a, int s) const 
{
    R = a;
    return R.shiftin(R, s);
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mulin( Rep& R, const Type_t& u ) const
{
	        for(typename Rep::iterator ri = R.begin();ri!=R.end();++ri)
			                _domain.mulin(*ri, u);
		    return R;

//  return _supportdomain.mulin(R,u);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mulin( Rep& R, const Rep& P ) const
{
  size_t sR = R.size(); 
  size_t sP = P.size(); 
  Rep tmp(sR+sP);
  mul(tmp, R, P);
//   R.logcopy(tmp);
//   return R;
  return assign(R,tmp);
}

// template <class Domain>
// Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mul( Rep& R, const Rep& P, const Rep& Q ) const
// {
//     size_t sR = R.size(); 
//     size_t sP = P.size(); 
//     size_t sQ = Q.size(); 
//     if ((sQ ==0) || (sP ==0)) { R.reallocate(0); return R; }
//     if (sR != sQ+sP) R.reallocate(sR = sP+sQ-1);
    
//     size_t i,j;
//     for (i=0; i<sR; ++i) _domain.init(R[i], _domain.zero);
//     for (i=0; i<sP; ++i) 
//         if (! _domain.isZero(P[i]))
//             for (j=0; j<sQ; ++j) 
//                 _domain.axpy(R[i+j], P[i], Q[j], R[i+j]);
//     return setdegree(R);
// }

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mul( Rep& R, const Rep& P, const Rep& Q ) const
{
	Type_t _zero;
	_domain.init( _zero, 0.0);
    size_t sR = R.size(); 
    size_t sP = P.size(); 
    size_t sQ = Q.size(); 
    if ((sQ ==0) || (sP ==0)) { R.reallocate(0); return R; }
    if (sR != sQ+sP) R.reallocate(sR = sP+sQ-1);
    
    typename Rep::const_iterator ai=P.begin(),bi=Q.begin();
    typename Rep::iterator ri=R.begin(), rig=R.begin();
    if (_domain.isZero(*ai))
        for(;bi!=Q.end();++bi,++ri)
            *ri = _zero;
    else
        for(;bi!=Q.end();++bi,++ri)
            if (_domain.isZero(*bi))
                *ri = _zero;
            else
                _domain.mul(*ri,*ai,*bi);
    for(;ri!=R.end();++ri)
        *ri = _zero;
    for(++ai,++rig;ai!=P.end();++ai,++rig)
        if (! _domain.isZero(*ai))
            for(ri=rig,bi=Q.begin();bi!=Q.end();++bi,++ri)
                _domain.axpyin(*ri,*ai,*bi);
    return setdegree(R);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mul
 ( Rep& R, const Rep& P, const Type_t& val ) const
{
    typename Rep::const_iterator ip = P.begin();
    R.resize(P.size());
    for(typename Rep::iterator ir = R.begin(); ir != R.end(); ++ir, ++ip)
        this->_domain.mul(*ir, *ip, val);
    return R;
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mul
 ( Rep& R, const Type_t& val, const Rep& P ) const
{
  return this->mul(R,P,val);
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::divin(Rep& R, const Type_t& u) const
{
#ifdef GIVARO_DEBUG
  if (_domain.isZero(u)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::divin]"));
#endif
  size_t sz =R.size();
  for (unsigned int i=0; i<sz; ++i)
    _domain.divin(R[i],u);
  return setdegree(R);
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::div(Rep& R, const Rep& P, const Type_t& u) const
{
#ifdef GIVARO_DEBUG
  if (_domain.isZero(u)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::div]"));
#endif
  size_t sP =P.size();
  R.reallocate(sP);
  for (unsigned int i=0; i<sP; ++i)
    _domain.div(R[i],P[i],u);
  return setdegree(R);
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::div(Rep& R, const Type_t& u, const Rep& P) const
{
#ifdef GIVARO_DEBUG
  if (isZero(P)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::divin]"));
#endif
  if (_domain.isZero(u)) { return assign(R,zero);}
  size_t sP =P.size();
  if (sP >1) { R.reallocate(0); return R; }
  size_t sR =R.size();
  if (sR !=1) R.reallocate(1);
  _domain.div(R[0], u, P[0]); 
  return setdegree(R);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::div(Rep& Q, const Rep& A, const Rep& B) const
{
  Rep R;
  return divmod(Q,R,A,B);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::divin(Rep& Q, const Rep& A) const
{
    Rep R, B;
    divmod(B,R,Q,A);
    return assign(Q,B);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::modin(Rep& R, const Type_t& u) const
{
#ifdef GIVARO_DEBUG
  if (_domain.isZero(u)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::modin]"));
#endif
  R.reallocate(0);
  return R;
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mod(Rep& R, const Rep& P, const Type_t& u) const
{
#ifdef GIVARO_DEBUG
  if (_domain.isZero(u)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::mod]"));
#endif
  R.reallocate(0);
  return R;
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mod(Rep& R, const Type_t& u, const Rep& P) const
{
#ifdef GIVARO_DEBUG
  if (isZero(P)) GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::mod]"));
#endif
  if (_domain.isZero(u)) { return assign(P,R); }
  size_t sP =P.size();
  if (sP >1) { 
    R.reallocate(1); 
    _domain.assign(R[0], u); 
    return R; 
  }
  R.reallocate(1);
  _domain.mod(R[0],u,P[0]);
  return R;
}


// template <class Domain>
// inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::modin(Rep& R, const Rep& A) const {
//     Rep tR; assign(tR,R);
//     return mod(R,tR,A);
// }

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::modin(Rep& A, const Rep& B) const {
        // In place remainder
        // A is written with next remainder in 
        // the division algorithm written at the end.
        // Last step is erasing of the first values.
//     write(std::cerr << "Rem(", A) << " ,";
//     write(std::cerr, B) << ", X) mod " << _domain.size();
	Type_t _zero;
	_domain.init( _zero, 0.0);
    long i = A.size()-B.size();
    if (i >= 0) {
        typedef typename Rep::value_type TT;
        TT l;
        typename Rep::reverse_iterator ai,aai;
        typename Rep::const_reverse_iterator bi;
        for (; i>=0; --i) {
            ai = A.rbegin();
            bi = B.rbegin();
            _domain.div(l,*ai,*bi);
            aai = A.rbegin();
            for(++bi,++ai;bi!=B.rend();++bi,++ai,--i) {
                _domain.amxy(*aai,l,*bi,*ai);
                if (! _domain.isZero(*aai)) break;
            }
            if (bi!=B.rend()) 
                for(++bi,++ai,++aai;bi!=B.rend();++bi,++ai,++aai)
                    _domain.amxy(*aai,l,*bi,*ai);
            for(;ai!=A.rend();++ai,++aai)
                *aai = *ai;
            *aai = _zero;
        }
//         write(std::cerr << " = ", A) << ";" << std::endl;
        A.erase(A.begin(), A.begin()+(A.size()-B.size()-i));
    }
//     write(std::cerr << " = ", setdegree(A)) << ";" << std::endl;
    return setdegree(A);
}

template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::mod(Rep& R, const Rep& A, const Rep& B) const
{
  Rep Q;
//   write(std::cerr, A) << " = (";
//   write(std::cerr, B) << ") * (";
  divmod(Q,R,A,B);
//   write(std::cerr, Q) << ") + (";
//   write(std::cerr, R) << ");" << std::endl;
  return R;
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::divmod( Rep& Q, Rep& R, const Rep& A, const Rep& B) const
// returns Q such that A = B Q + R
{
  Degree degB; degree(degB, B);
	Type_t _zero;
	_domain.init( _zero, 0.0); 
#ifdef GIVARO_DEBUG
  if (degB == Degree::deginfty) 
    GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::div]"));
#endif
  Degree degA; degree(degA, A); 
  if (degA == Degree::deginfty) {
    assign(R, zero);
    return assign(Q, zero);
  }
  if (degB == 0) // cste
  {
    assign(R, zero);
    return div(Q, A, B[0]);
  }
// JGD 15.12.1999 : 
//   if (degA ==0)
//   {
//     assign(R, zero);
//     return assign(Q, zero);
//   }
  if (degB > degA) {
    assign(R, A);
    return assign(Q, zero);
  }

  long degQuo = value(degA-degB);
  long degRem = value(degA);
  Q.reallocate(degQuo+1);

  assign(R,A);

  Type_t tmp;
  long i,j;
  for (i=degQuo; i>=0; --i) 
  {
    // == ld X^ (degRem-degQ)
    _domain.div(Q[degQuo], R[degRem], B[degB.value()]);

    _domain.neg(tmp, Q[degQuo]);
    for (j=0; degB>j; j++) { // rem <- rem - ld*x^(degRem-degB)*B
      _domain.axpyin(R[j+degQuo], tmp, B[j]);
    }
    _domain.assign(R[degRem],_zero) ; degQuo--; degRem--;
  }
  R.reallocate(degRem+1);
  setdegree(R);
  return setdegree(Q);
}


template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::pdivmod
 ( Rep& Q, Rep& R, Type_t& m, const Rep& A, const Rep& B) const
// returns Q ...
{
	Type_t _zero, _one;
	_domain.init( _one, 1.0);
	_domain.init( _zero, 0.0);
  Degree degB; degree(degB, B); 
#ifdef GIVARO_DEBUG
  if (degB == Degree::deginfty) 
    GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::div]"));
#endif
  Degree degA; degree(degA, A);
  if (degA == Degree::deginfty) {
    assign(R, zero);
    _domain.assign(m, _domain.one);
    return assign(Q, zero);
  }
  if (degB == 0) // cste
  {
    assign(R, zero);
    _domain.assign(m, B[0]);
    return assign(Q, A);
  }
  if (degA ==0)
  {
    assign(R, zero);
    _domain.assign(m, _domain.one);
    return assign(Q, zero);
  }
  if (degB > degA) {
    assign(R, A);
    _domain.assign(m, _domain.one);
    return assign(Q, zero);
  }

  long degQuo = value(degA-degB);
  long degRem = value(degA);
  Q.reallocate(degQuo+1);
  assign(R,A);

  Type_t tmp, lB;
  _domain.assign(lB, B[degB.value()]); 
  _domain.assign(m, _domain.one); 
  long i,j;
  for (i=degQuo; i>=0; --i)
  {
    // == ld X^ (degRem-degQ)
    _domain.assign(Q[degQuo], R[degRem]);

    // rem <- lB*rem - lQ*x^(degRem-degB)*B
    for (j=0; j<degQuo; j++)
      _domain.mulin (R[j], lB);
    for (j=0; degB>j; j++) 
    { 
      _domain.mulin(R[j+degQuo], lB);
      _domain.axmyin(R[j+degQuo], Q[degQuo], B[j]);
    }
    _domain.assign(R[degRem],_zero); degQuo--; degRem--;
    _domain.mulin(m, lB);
  }
  R.reallocate(degRem+1);
  setdegree(R);
  return setdegree(Q);
//  Poly1Dom<Domain,Dense>::Rep U,V;
//  assign(U,A);
//  mulin(U,m);
//  write(std::cout << "m*A:", U) << std::endl;
//  mul(U,Q,B);
//  write(std::cout << "Q*B:", U) << std::endl;
}



template <class Domain>
inline typename Poly1Dom<Domain,Dense>::Rep& Poly1Dom<Domain,Dense>::pmod
 ( Rep& R, Type_t& m, const Rep& A, const Rep& B) const
{
  Degree degB; degree(degB, B);
#ifdef GIVARO_DEBUG
  if (degB == Degree::deginfty) 
    GivError::throw_error(GivMathDivZero("[Poly1Dom<D>::div]"));
#endif
  Degree degA; degree(degA, A); 
  if (degA == Degree::deginfty) {
    _domain.assign(m, _domain.one);
    return assign(R, zero);
  }
  if (degB == 0) // cste
  {
    _domain.assign(m, B[0]);
    return assign(R, zero);
  }
  if (degA ==0)
  {
    _domain.assign(m, _domain.one);
    return assign(R, zero);
  }
  if (degB > degA) {
    _domain.assign(m, _domain.one);
    return assign(R, A);
  }

  Degree degR = degA;
  assign(R,A);

  Type_t tmp, lB;
  _domain.assign(lB, B[degB.value()]);
  long i,j;
//write(std::cout << "B:", B) << std::endl;
//_domain.write(std::cout << "lB:", lB) << "^" << degA-degB+1 << std::endl;
//   _domain.pow(m, lB, degA.value()-degB.value()+1);
  dom_power(m, lB, degA.value()-degB.value()+1,_domain);
//_domain.write(std::cout << "m:", m) << std::endl;
  for (; degB<= degR; )
  {
    long d = degR.value()-degB.value();
    // R <- lB*R - lR*x^(degR-degB)*B
    for (j=0; degB>j; j++)
    {
      _domain.mulin (R[j+d], lB);
      _domain.axmyin(R[j+d], R[degR.value()], B[j]);
    }
    for (j=0; j<d; ++j)
      _domain.mulin (R[j], lB);
    _domain.assign(R[degR.value()],_domain.zero); 
    degree(degR, R); 
  }
  R.reallocate(degR.value()+1);
  return setdegree(R);
}
