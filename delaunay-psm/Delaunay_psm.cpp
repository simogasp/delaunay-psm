#include "Delaunay_psm.h"

/*
 *  Copyright (c) 2012-2014, Bruno Levy
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  * Neither the name of the ALICE Project-Team nor the names of its
 *  contributors may be used to endorse or promote products derived from this
 *  software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  If you modify this software, you should include a notice giving the
 *  name of the person performing the modification, the date of modification,
 *  and the reason for such modification.
 *
 *  Contact: Bruno Levy
 *
 *     Bruno.Levy@inria.fr
 *     http://www.loria.fr/~levy
 *
 *     ALICE Project
 *     LORIA, INRIA Lorraine, 
 *     Campus Scientifique, BP 239
 *     54506 VANDOEUVRE LES NANCY CEDEX 
 *     FRANCE
 *
 */


/*
 *  This file is a PSM (pluggable software module)
 *   generated from the distribution of Geogram.
 *
 *  See Geogram documentation on:
 *   http://alice.loria.fr/software/geogram/doc/html/index.html
 *
 *  See documentation of the functions bundled in this PSM on:
 *   http://alice.loria.fr/software/geogram/doc/html/classGEO_1_1Delaunay.html
 */



/******* extracted from ../basic/memory.h *******/

#ifndef GEOGRAM_BASIC_MEMORY
#define GEOGRAM_BASIC_MEMORY

#include <vector>
#include <string.h>
#include <stdlib.h>

#ifdef GEO_OS_WINDOWS

#include <windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#else

#include <unistd.h>

#endif


namespace GEO {

    namespace Memory {
        
        typedef unsigned char byte;

        
        typedef unsigned char word8;

        
        typedef unsigned short word16;

        
        typedef unsigned int word32;

        
        typedef byte* pointer;

	
	typedef void (*function_pointer)();
	
        inline void clear(void* addr, size_t size) {
            ::memset(addr, 0, size);
        }

        inline void copy(void* to, const void* from, size_t size) {
            ::memcpy(to, from, size);
        }

	inline pointer function_pointer_to_generic_pointer(function_pointer fptr) {
	    // I know this is ugly, but I did not find a simpler warning-free
	    // way that is portable between all compilers.
	    pointer result = nullptr;
	    ::memcpy(&result, &fptr, sizeof(pointer));
	    return result;
	}

	inline function_pointer generic_pointer_to_function_pointer(pointer ptr) {
	    // I know this is ugly, but I did not find a simpler warning-free
	    // way that is portable between all compilers.
	    function_pointer result = nullptr;
	    ::memcpy(&result, &ptr, sizeof(pointer));
	    return result;
	}

	inline function_pointer generic_pointer_to_function_pointer(void* ptr) {
	    // I know this is ugly, but I did not find a simpler warning-free
	    // way that is portable between all compilers.
	    function_pointer result = nullptr;
	    ::memcpy(&result, &ptr, sizeof(pointer));
	    return result;
	}
	
#define GEO_MEMORY_ALIGNMENT 64

        template <int DIM>
        struct PointAlignment {
            static const size_t value = 1;
        };

        template <>
        struct PointAlignment<2> {
            static const size_t value = 16;
        };

        template <>
        struct PointAlignment<3> {
            static const size_t value = 8;
        };

        template <>
        struct PointAlignment<4> {
            static const size_t value = 32;
        };

        template <>
        struct PointAlignment<6> {
            static const size_t value = 16;
        };

        template <>
        struct PointAlignment<8> {
            static const size_t value = 64;
        };

#define geo_dim_alignment(dim) GEO::Memory::PointAlignment<dim>::value

        inline void* aligned_malloc(
            size_t size, size_t alignment = GEO_MEMORY_ALIGNMENT
        ) {
#if   defined(GEO_OS_ANDROID)
            // Alignment not supported under Android.
            geo_argused(alignment);
            return malloc(size);
#elif defined(GEO_COMPILER_INTEL)
            return _mm_malloc(size, alignment);
#elif defined(GEO_COMPILER_GCC) || defined(GEO_COMPILER_CLANG)
            void* result;
            return posix_memalign(&result, alignment, size) == 0
                   ? result : nullptr;
#elif defined(GEO_COMPILER_MSVC)
            return _aligned_malloc(size, alignment);
#else
            geo_argused(alignment);
            return malloc(size);
#endif
        }

        inline void aligned_free(void* p) {
#if   defined(GEO_OS_ANDROID)
            // Alignment not supported under Android.
            free(p);
#elif defined(GEO_COMPILER_INTEL)
            _mm_free(p);
#elif defined(GEO_COMPILER_GCC_FAMILY) 
            free(p);
#elif defined(GEO_COMPILER_MSVC)
            _aligned_free(p);
#else
            free(p);
#endif
        }

#if   defined(GEO_OS_ANDROID)
#define geo_decl_aligned(var) var
#elif defined(GEO_COMPILER_INTEL)
#define geo_decl_aligned(var) __declspec(aligned(GEO_MEMORY_ALIGNMENT)) var
#elif defined(GEO_COMPILER_GCC_FAMILY)
#define geo_decl_aligned(var) var __attribute__((aligned(GEO_MEMORY_ALIGNMENT)))
#elif defined(GEO_COMPILER_MSVC)
#define geo_decl_aligned(var) __declspec(align(GEO_MEMORY_ALIGNMENT)) var
#elif defined(GEO_COMPILER_EMSCRIPTEN)
#define geo_decl_aligned(var) var        
#endif

#if   defined(GEO_OS_ANDROID)
#define geo_assume_aligned(var, alignment)
#elif defined(GEO_COMPILER_INTEL)
#define geo_assume_aligned(var, alignment) \
    __assume_aligned(var, alignment)
#elif defined(GEO_COMPILER_CLANG)
#define geo_assume_aligned(var, alignment)
        // GCC __builtin_assume_aligned is not yet supported by clang-3.3
#elif defined(GEO_COMPILER_GCC)
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#define geo_assume_aligned(var, alignment) \
        *(void**) (&var) = __builtin_assume_aligned(var, alignment)
        // the GCC way of specifying that a pointer is aligned returns
        // the aligned pointer (I can't figure out why). It needs to be
        // affected otherwise it is not taken into account (verified by
        // looking at the output of gcc -S)
#else
#define geo_assume_aligned(var, alignment)        
#endif        
#elif defined(GEO_COMPILER_MSVC) 
#define geo_assume_aligned(var, alignment)
        // TODO: I do not know how to do that with MSVC
#elif defined(GEO_COMPILER_EMSCRIPTEN)        
#define geo_assume_aligned(var, alignment)
#elif defined(GEO_COMPILER_MINGW)        
#define geo_assume_aligned(var, alignment)
#endif

#if   defined(GEO_COMPILER_INTEL)
#define geo_restrict __restrict
#elif defined(GEO_COMPILER_GCC_FAMILY)
#define geo_restrict __restrict__
#elif defined(GEO_COMPILER_MSVC)
#define geo_restrict __restrict
#elif defined(GEO_COMPILER_EMSCRIPTEN)
#define geo_restrict 
#endif

        inline bool is_aligned(
            void* p, size_t alignment = GEO_MEMORY_ALIGNMENT
        ) {
            return (reinterpret_cast<size_t>(p) & (alignment - 1)) == 0;
        }

        inline void* align(void* p) {
            size_t offset = (
                GEO_MEMORY_ALIGNMENT -
                (reinterpret_cast<size_t>(p) & (GEO_MEMORY_ALIGNMENT - 1))
            ) & (GEO_MEMORY_ALIGNMENT - 1);
            return reinterpret_cast<char*>(p) + offset;
        }

#define geo_aligned_alloca(size) \
    GEO::Memory::align(alloca(size + GEO_MEMORY_ALIGNMENT - 1))

        template <class T, int ALIGN = GEO_MEMORY_ALIGNMENT>
        class aligned_allocator {
        public:
            
            typedef T value_type;

            
            typedef T* pointer;

            
            typedef T& reference;

            
            typedef const T* const_pointer;

            
            typedef const T& const_reference;

            
            typedef ::std::size_t size_type;

            
            typedef ::std::ptrdiff_t difference_type;

            template <class U>
            struct rebind {
                
                typedef aligned_allocator<U> other;
            };

            pointer address(reference x) {
                return &x;
            }

            const_pointer address(const_reference x) {
                return &x;
            }

            pointer allocate(
                size_type nb_elt, ::std::allocator<void>::const_pointer hint = nullptr
            ) {
                geo_argused(hint);
                pointer result = static_cast<pointer>(
                    aligned_malloc(sizeof(T) * nb_elt, ALIGN)
                );
                return result;
            }

            void deallocate(pointer p, size_type nb_elt) {
                geo_argused(nb_elt);
                aligned_free(p);
            }

            size_type max_size() const {
                ::std::allocator<char> a;
                return a.max_size() / sizeof(T);
            }

            void construct(pointer p, const_reference val) {
                new (static_cast<void*>(p))value_type(val);
            }

            void destroy(pointer p) {
                p->~value_type();
#ifdef GEO_COMPILER_MSVC
                (void) p; // to avoid a "unreferenced variable" warning
#endif
            }

            template <class T2, int A2> operator aligned_allocator<T2, A2>() {
                return aligned_allocator<T2,A2>();
            }
        };

        template <typename T1, int A1, typename T2, int A2>
        inline bool operator== (
            const aligned_allocator<T1, A1>&, const aligned_allocator<T2, A2>&
        ) {
            return true;
        }

        template <typename T1, int A1, typename T2, int A2>
        inline bool operator!= (
            const aligned_allocator<T1, A1>&, const aligned_allocator<T2, A2>&
        ) {
            return false;
        }
    }

    

    template <class T>
    class vector : public ::std::vector<T, Memory::aligned_allocator<T> > {
        typedef ::std::vector<T, Memory::aligned_allocator<T> > baseclass;

    public:
        vector() :
            baseclass() {
        }

        explicit vector(index_t size) :
            baseclass(size) {
        }

        explicit vector(index_t size, const T& val) :
            baseclass(size, val) {
        }

        index_t size() const {
            //   casts baseclass::size() from size_t (64 bits)
            //   to index_t (32 bits), because all
            //   indices in Vorpaline are supposed to fit in 32 bits (index_t).
            // TODO: geo_debug_assert(baseclass::size() < max index_t)
            return index_t(baseclass::size());
        }

        T& operator[] (index_t i) {
            geo_debug_assert(i < size());
            return baseclass::operator[] (i);
        }

        const T& operator[] (index_t i) const {
            geo_debug_assert(i < size());
            return baseclass::operator[] (i);
        }

        T& operator[] (signed_index_t i) {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }

        const T& operator[] (signed_index_t i) const {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }


#ifdef GARGANTUA // If compiled with 64 bits index_t

        T& operator[] (int i) {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }

        const T& operator[] (int i) const {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }

        T& operator[] (unsigned int i) {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }

        const T& operator[] (unsigned int i) const {
            geo_debug_assert(i >= 0 && index_t(i) < size());
            return baseclass::operator[] (index_t(i));
        }
#endif	
	
        T* data() {
            return size() == 0 ? nullptr : &(*this)[0];
        }

        const T* data() const {
            return size() == 0 ? nullptr : &(*this)[0];
        }

    };

    template <>
    class vector<bool> : public ::std::vector<bool> {
        typedef ::std::vector<bool> baseclass;

    public:
        
        vector() :
            baseclass() {
        }

        
        explicit vector(index_t size) :
            baseclass(size) {
        }

        
        explicit vector(index_t size, bool val) :
            baseclass(size, val) {
        }

        
        index_t size() const {
            //   casts baseclass::size() from size_t (64 bits)
            //   to index_t (32 bits), because all
            //   indices in Vorpaline are supposed to fit in 32 bits (index_t).
            // TODO: geo_debug_assert(baseclass::size() < max index_t)
            return index_t(baseclass::size());
        }

        // TODO: operator[] with bounds checking (more complicated
        // than just returning bool&, check implementation in STL).
    };
}

#endif


/******* extracted from ../basic/matrix.h *******/

#ifndef GEOGRAM_BASIC_MATRIX
#define GEOGRAM_BASIC_MATRIX



namespace GEO {

    

    inline double det2x2(
        double a11, double a12,                    
        double a21, double a22
    ) {                                 
        return a11*a22-a12*a21 ;
    }

    inline double det3x3(
        double a11, double a12, double a13,                
        double a21, double a22, double a23,                
        double a31, double a32, double a33
    ) {
    return
         a11*det2x2(a22,a23,a32,a33)   
        -a21*det2x2(a12,a13,a32,a33)   
        +a31*det2x2(a12,a13,a22,a23);
    }   


    inline double det4x4(
        double a11, double a12, double a13, double a14,
        double a21, double a22, double a23, double a24,               
        double a31, double a32, double a33, double a34,  
        double a41, double a42, double a43, double a44  
    ) {
        double m12 = a21*a12 - a11*a22;
        double m13 = a31*a12 - a11*a32;
        double m14 = a41*a12 - a11*a42;
        double m23 = a31*a22 - a21*a32;
        double m24 = a41*a22 - a21*a42;
        double m34 = a41*a32 - a31*a42;

        double m123 = m23*a13 - m13*a23 + m12*a33;
        double m124 = m24*a13 - m14*a23 + m12*a43;
        double m134 = m34*a13 - m14*a33 + m13*a43;
        double m234 = m34*a23 - m24*a33 + m23*a43;
        
        return (m234*a14 - m134*a24 + m124*a34 - m123*a44);
    }   


    template <index_t DIM, class FT>
    class Matrix {
    public:
        
        typedef Matrix<DIM, FT> matrix_type;

        
        typedef FT value_type;

        
        static const index_t dim = DIM;

        inline Matrix() {
            load_identity();
        }

        explicit Matrix(const FT* vals) {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] = *vals;
                    ++vals;
                }
            }
        }
        
        inline index_t dimension() const {
            return DIM;
        }

        inline void load_zero() {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] = FT(0);
                }
            }
        }

        inline void load_identity() {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] = (i == j) ? FT(1) : FT(0);
                }
            }
        }

        inline bool is_identity() const {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    FT rhs = ((i == j) ? FT(1) : FT(0));
                    if(coeff_[i][j] != rhs) {
                        return false;
                    }
                }
            }
            return true;
        }
        
        inline FT& operator() (index_t i, index_t j) {
            geo_debug_assert(i < DIM);
            geo_debug_assert(j < DIM);
            return coeff_[i][j];
        }

        inline const FT& operator() (index_t i, index_t j) const {
            geo_debug_assert(i < DIM);
            geo_debug_assert(j < DIM);
            return coeff_[i][j];
        }

        inline matrix_type& operator+= (const matrix_type& m) {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] += m.coeff_[i][j];
                }
            }
            return *this;
        }

        inline matrix_type& operator-= (const matrix_type& m) {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] -= m.coeff_[i][j];
                }
            }
            return *this;
        }

        inline matrix_type& operator*= (FT val) {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] *= val;
                }
            }
            return *this;
        }

        inline matrix_type& operator/= (FT val) {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    coeff_[i][j] /= val;
                }
            }
            return *this;
        }

        inline matrix_type operator+ (const matrix_type& m) const {
            matrix_type result = *this;
            result += m;
            return result;
        }

        inline matrix_type operator- (const matrix_type& m) const {
            matrix_type result = *this;
            result -= m;
            return result;
        }

        inline matrix_type operator* (FT val) const {
            matrix_type result = *this;
            result *= val;
            return result;
        }

        inline matrix_type operator/ (FT val) const {
            matrix_type result = *this;
            result /= val;
            return result;
        }

        matrix_type operator* (const matrix_type& m) const {
            matrix_type result;
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    result.coeff_[i][j] = FT(0);
                    for(index_t k = 0; k < DIM; k++) {
                        result.coeff_[i][j] += coeff_[i][k] * m.coeff_[k][j];
                    }
                }
            }
            return result;
        }

        matrix_type inverse() const {
            matrix_type result;
            bool invertible = compute_inverse(result);
            geo_assert(invertible);
            return result;
        }


        bool compute_inverse(matrix_type& result) const {
            FT val=FT(0.0), val2=FT(0.0);
            matrix_type tmp = (*this);

            result.load_identity();

            for(index_t i = 0; i != DIM; i++) {
                val = tmp(i, i);                     /* find pivot */
                index_t ind = i;
                for(index_t j = i + 1; j != DIM; j++) {
                    if(fabs(tmp(j, i)) > fabs(val)) {
                        ind = j;
                        val = tmp(j, i);
                    }
                }

                if(ind != i) {
                    for(index_t j = 0; j != DIM; j++) {
                        val2 = result(i, j);
                        result(i, j) = result(ind, j);
                        result(ind, j) = val2;           /* swap columns */
                        val2 = tmp(i, j);
                        tmp(i, j) = tmp(ind, j);
                        tmp(ind, j) = val2;
                    }
                }

                if(val == 0.0) {
                    return false;
                }

                for(index_t j = 0; j != DIM; j++) {
                    tmp(i, j) /= val;
                    result(i, j) /= val;
                }

                for(index_t j = 0; j != DIM; j++) {
                    if(j == i) {
                        continue;                       /* eliminate column */
                    }
                    val = tmp(j, i);
                    for(index_t k = 0; k != DIM; k++) {
                        tmp(j, k) -= tmp(i, k) * val;
                        result(j, k) -= result(i, k) * val;
                    }
                }
            }
            
            return true;
        }

        matrix_type transpose() const {
            matrix_type result;
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j < DIM; j++) {
                    result(i, j) = (* this)(j, i);
                }
            }
            return result;
        }

        

        inline const FT* data() const {
            return &(coeff_[0][0]);
        }

        

        inline FT* data() {
            return &(coeff_[0][0]);
        }

        void get_lower_triangle(FT* store) const {
            for(index_t i = 0; i < DIM; i++) {
                for(index_t j = 0; j <= i; j++) {
                    *store++ = coeff_[i][j];
                }
            }
        }

    private:
        FT coeff_[DIM][DIM];
    };

    

    template <index_t DIM, class FT>
    inline std::ostream& operator<< (
        std::ostream& output, const Matrix<DIM, FT>& m
    ) {
        const char* sep = "";
        for(index_t i = 0; i < DIM; i++) {
            for(index_t j = 0; j < DIM; j++) {
                output << sep << m(i, j);
                sep = " ";
            }
        }
        return output;
    }

    template <index_t DIM, class FT>
    inline std::istream& operator>> (
        std::istream& input, Matrix<DIM, FT>& m
    ) {
        for(index_t i = 0; i < DIM; i++) {
            for(index_t j = 0; j < DIM; j++) {
                input >> m(i, j);
            }
        }
        return input;
    }

    

    template <index_t DIM, class FT> inline
    void mult(const Matrix<DIM, FT>& M, const FT* x, FT* y) {
        for(index_t i = 0; i < DIM; i++) {
            y[i] = 0;
            for(index_t j = 0; j < DIM; j++) {
                y[i] += M(i, j) * x[j];
            }
        }
    }

    

    template <index_t DIM, class FT> inline
    vecng<DIM,FT> operator*(
        const Matrix<DIM, FT>& M, const vecng<DIM,FT>& x
    ) {
        vecng<DIM,FT> y;
        for(index_t i = 0; i < DIM; i++) {
            y[i] = 0;
            for(index_t j = 0; j < DIM; j++) {
                y[i] += M(i, j) * x[j];
            }
        }
        return y;
    }

    
    
}

#endif


/******* extracted from ../basic/geometry_nd.h *******/

#ifndef GEOGRAM_BASIC_GEOMETRY_ND
#define GEOGRAM_BASIC_GEOMETRY_ND



namespace GEO {

    namespace Geom {

        template <class COORD_T>
        inline double distance2(
            const COORD_T* p1, const COORD_T* p2, coord_index_t dim
        ) {
            double result = 0.0;
            for(coord_index_t i = 0; i < dim; i++) {
                result += GEO::geo_sqr(double(p2[i]) - double(p1[i]));
            }
            return result;
        }

        template <class COORD_T>
        inline double distance(
            const COORD_T* p1, const COORD_T* p2, coord_index_t dim
        ) {
            return ::sqrt(distance2(p1, p2, dim));
        }

        template <class VEC>
        inline double distance2(
            const VEC& p1, const VEC& p2
        ) {
            geo_debug_assert(p1.dimension() == p2.dimension());
            return distance2(
                p1.data(), p2.data(), coord_index_t(p1.dimension())
            );
        }

        template <class VEC>
        inline double distance(
            const VEC& p1, const VEC& p2
        ) {
            geo_debug_assert(p1.dimension() == p2.dimension());
            return distance(p1.data(), p2.data(), p1.dimension());
        }

        template <class COORD_T>
        inline double triangle_area(
            const COORD_T* p1,
            const COORD_T* p2,
            const COORD_T* p3,
            coord_index_t dim
        ) {
            double a = distance(p1, p2, dim);
            double b = distance(p2, p3, dim);
            double c = distance(p3, p1, dim);
            double s = double(0.5) * (a + b + c);
            double A2 = s * (s - a) * (s - b) * (s - c);
            // the max is there to avoid some numerical problems.
            return ::sqrt(std::max(A2, 0.0));
        }

        template <class COORD_T>
        inline void triangle_centroid(
            const COORD_T* p,
            const COORD_T* q,
            const COORD_T* r,
            COORD_T a, COORD_T b, COORD_T c,
            double* Vg,
            double& V,
            coord_index_t dim
        ) {
            double abc = a + b + c;
            double area = Geom::triangle_area(p, q, r, dim);
            V = area / 3.0 * abc;
            double wp = a + abc;
            double wq = b + abc;
            double wr = c + abc;
            double s = area / 12.0;
            for(coord_index_t i = 0; i < dim; i++) {
                Vg[i] = s * (wp * p[i] + wq * q[i] + wr * r[i]);
            }
        }

        

        template <class VEC>
        inline double triangle_area(
            const VEC& p1, const VEC& p2, const VEC& p3
        ) {
            // Heron formula
            double a = distance(p1, p2);
            double b = distance(p2, p3);
            double c = distance(p3, p1);
            double s = double(0.5) * (a + b + c);
            return ::sqrt(s * (s - a) * (s - b) * (s - c));
        }

        template <class VEC>
        inline double triangle_mass(
            const VEC& p, const VEC& q, const VEC& r,
            double a, double b, double c
        ) {
            // TODO: try to better understand the formula and
            // determine why there are these sqrt's
            // (probably due to the relation between the
            //  user-provided density and the one achieved
            //  by CVT), but I'm pretty sure that the formula
            //  is correct (at least, dimensions match).
            // Note: the ::fabs() are there to avoid numerical
            // errors.
            return Geom::triangle_area(p, q, r) / 3.0 * (
                ::sqrt(::fabs(a)) + sqrt(::fabs(b)) + sqrt(::fabs(c))
            );
        }

        template <class POINT>
        POINT triangle_circumcenter(
            const POINT& Q1,
            const POINT& Q2,
            const POINT& Q3,
            double* denom = nullptr
        ) {
            const POINT q2 = Q2 - Q1;
            const POINT q3 = Q3 - Q1;

            double l2 = length2(q2);
            double l3 = length2(q3);

            double a12 = -2.0 * dot(q2, q2);
            double a13 = -2.0 * dot(q3, q2);
            double a22 = -2.0 * dot(q2, q3);
            double a23 = -2.0 * dot(q3, q3);

            double c31 = (a23 * a12 - a22 * a13);
            double d = c31;
            double s = 1.0 / d;
            double lambda1 = s * ((a23 - a22) * l2 + (a12 - a13) * l3 + c31);
            double lambda2 = s * ((-a23) * l2 + (a13) * l3);
            double lambda3 = s * ((a22) * l2 + (-a12) * l3);
            if(denom != nullptr) {
                *denom = d;
            }
            return lambda1 * Q1 + lambda2 * Q2 + lambda3 * Q3;
        }

        template <class VEC>
        inline void triangle_centroid(
            const VEC& p, const VEC& q, const VEC& r,
            double a, double b, double c,
            VEC& Vg, double& V
        ) {
            double abc = a + b + c;
            double area = Geom::triangle_area(p, q, r);
            V = area / 3.0 * abc;
            double wp = a + abc;
            double wq = b + abc;
            double wr = c + abc;
            double s = area / 12.0;
            Vg = s * (wp * p + wq * q + wr * r);
        }

        template <class VEC>
        inline VEC random_point_in_triangle(
            const VEC& p1, const VEC& p2, const VEC& p3
        ) {
            double l1 = Numeric::random_float64();
            double l2 = Numeric::random_float64();
            if(l1 + l2 > 1.0) {
                l1 = 1.0 - l1;
                l2 = 1.0 - l2;
            }
            double l3 = 1.0 - l1 - l2;
            return l1 * p1 + l2 * p2 + l3 * p3;
        }

        template <class VEC>
        inline VEC random_point_in_tetra(
            const VEC& p1, const VEC& p2, const VEC& p3, const VEC& p4
        ) {
            double s = Numeric::random_float64();
            double t = Numeric::random_float64();
            double u = Numeric::random_float64();
            if(s + t > 1.0) {
                s = 1.0 - s;
                t = 1.0 - t;
            }
            if(t + u > 1.0) {
                double tmp = u;
                u = 1.0 - s - t;
                t = 1.0 - tmp;
            } else if(s + t + u > 1.0) {
                double tmp = u;
                u = s + t + u - 1.0;
                s = 1.0 - t - tmp;
            }
            double a = 1.0 - s - t - u;
            return a * p1 + s * p2 + t * p3 + u * p4;
        }

        template <class VEC>
        inline double point_segment_squared_distance(
            const VEC& point,
            const VEC& V0,
            const VEC& V1,
            VEC& closest_point,
            double& lambda0,
            double& lambda1
        ) {
            double l2 = distance2(V0,V1);
            double t = dot(point - V0, V1 - V0);
            if(t <= 0.0 || l2 == 0.0) {
                closest_point = V0;
                lambda0 = 1.0;
                lambda1 = 0.0;
                return distance2(point, V0);
            } else if(t > l2) {
                closest_point = V1;
                lambda0 = 0.0;
                lambda1 = 1.0;
                return distance2(point, V1);
            } 
            lambda1 = t / l2;
            lambda0 = 1.0-lambda1;
            closest_point = lambda0 * V0 + lambda1 * V1;
            return distance2(point, closest_point);
        }
        

        template <class VEC>
        inline double point_segment_squared_distance(
            const VEC& point,
            const VEC& V0,
            const VEC& V1
        ) {
            VEC closest_point;
            double lambda0;
            double lambda1;
            return point_segment_squared_distance(
                point, V0, V1, closest_point, lambda0, lambda1
            );
        }
        
        template <class VEC>
        inline double point_triangle_squared_distance(
            const VEC& point,
            const VEC& V0,
            const VEC& V1,
            const VEC& V2,
            VEC& closest_point,
            double& lambda0, double& lambda1, double& lambda2
        ) {
            VEC diff = V0 - point;
            VEC edge0 = V1 - V0;
            VEC edge1 = V2 - V0;
            double a00 = length2(edge0);
            double a01 = dot(edge0, edge1);
            double a11 = length2(edge1);
            double b0 = dot(diff, edge0);
            double b1 = dot(diff, edge1);
            double c = length2(diff);
            double det = ::fabs(a00 * a11 - a01 * a01);
            double s = a01 * b1 - a11 * b0;
            double t = a01 * b0 - a00 * b1;
            double sqrDistance;

	    // If the triangle is degenerate
	    if(det < 1e-30) {
		double cur_l1, cur_l2;
		VEC cur_closest;
		double result;
		double cur_dist = point_segment_squared_distance(point, V0, V1, cur_closest, cur_l1, cur_l2);
		result = cur_dist;
		closest_point = cur_closest;
		lambda0 = cur_l1;
		lambda1 = cur_l2;
		lambda2 = 0.0;
		cur_dist = point_segment_squared_distance(point, V0, V2, cur_closest, cur_l1, cur_l2);
		if(cur_dist < result) {
		    result = cur_dist;
		    closest_point = cur_closest;
		    lambda0 = cur_l1;
		    lambda2 = cur_l2;
		    lambda1 = 0.0;
		}
		cur_dist = point_segment_squared_distance(point, V1, V2, cur_closest, cur_l1, cur_l2);
		if(cur_dist < result) {
		    result = cur_dist;
		    closest_point = cur_closest;
		    lambda1 = cur_l1;
		    lambda2 = cur_l2;
		    lambda0 = 0.0;
		}
		return result;
	    }
	    
            if(s + t <= det) {
                if(s < 0.0) {
                    if(t < 0.0) {   // region 4
                        if(b0 < 0.0) {
                            t = 0.0;
                            if(-b0 >= a00) {
                                s = 1.0;
                                sqrDistance = a00 + 2.0 * b0 + c;
                            } else {
                                s = -b0 / a00;
                                sqrDistance = b0 * s + c;
                            }
                        } else {
                            s = 0.0;
                            if(b1 >= 0.0) {
                                t = 0.0;
                                sqrDistance = c;
                            } else if(-b1 >= a11) {
                                t = 1.0;
                                sqrDistance = a11 + 2.0 * b1 + c;
                            } else {
                                t = -b1 / a11;
                                sqrDistance = b1 * t + c;
                            }
                        }
                    } else {  // region 3
                        s = 0.0;
                        if(b1 >= 0.0) {
                            t = 0.0;
                            sqrDistance = c;
                        } else if(-b1 >= a11) {
                            t = 1.0;
                            sqrDistance = a11 + 2.0 * b1 + c;
                        } else {
                            t = -b1 / a11;
                            sqrDistance = b1 * t + c;
                        }
                    }
                } else if(t < 0.0) {  // region 5
                    t = 0.0;
                    if(b0 >= 0.0) {
                        s = 0.0;
                        sqrDistance = c;
                    } else if(-b0 >= a00) {
                        s = 1.0;
                        sqrDistance = a00 + 2.0 * b0 + c;
                    } else {
                        s = -b0 / a00;
                        sqrDistance = b0 * s + c;
                    }
                } else {  // region 0
                    // minimum at interior point
                    double invDet = double(1.0) / det;
                    s *= invDet;
                    t *= invDet;
                    sqrDistance = s * (a00 * s + a01 * t + 2.0 * b0) +
                        t * (a01 * s + a11 * t + 2.0 * b1) + c;
                }
            } else {
                double tmp0, tmp1, numer, denom;

                if(s < 0.0) {   // region 2
                    tmp0 = a01 + b0;
                    tmp1 = a11 + b1;
                    if(tmp1 > tmp0) {
                        numer = tmp1 - tmp0;
                        denom = a00 - 2.0 * a01 + a11;
                        if(numer >= denom) {
                            s = 1.0;
                            t = 0.0;
                            sqrDistance = a00 + 2.0 * b0 + c;
                        } else {
                            s = numer / denom;
                            t = 1.0 - s;
                            sqrDistance = s * (a00 * s + a01 * t + 2.0 * b0) +
                                t * (a01 * s + a11 * t + 2.0 * b1) + c;
                        }
                    } else {
                        s = 0.0;
                        if(tmp1 <= 0.0) {
                            t = 1.0;
                            sqrDistance = a11 + 2.0 * b1 + c;
                        }
                        else if(b1 >= 0.0) {
                            t = 0.0;
                            sqrDistance = c;
                        } else {
                            t = -b1 / a11;
                            sqrDistance = b1 * t + c;
                        }
                    }
                } else if(t < 0.0) {  // region 6
                    tmp0 = a01 + b1;
                    tmp1 = a00 + b0;
                    if(tmp1 > tmp0) {
                        numer = tmp1 - tmp0;
                        denom = a00 - 2.0 * a01 + a11;
                        if(numer >= denom) {
                            t = 1.0;
                            s = 0.0;
                            sqrDistance = a11 + 2.0 * b1 + c;
                        } else {
                            t = numer / denom;
                            s = 1.0 - t;
                            sqrDistance = s * (a00 * s + a01 * t + 2.0 * b0) +
                                t * (a01 * s + a11 * t + 2.0 * b1) + c;
                        }
                    } else {
                        t = 0.0;
                        if(tmp1 <= 0.0) {
                            s = 1.0;
                            sqrDistance = a00 + 2.0 * b0 + c;
                        } else if(b0 >= 0.0) {
                            s = 0.0;
                            sqrDistance = c;
                        } else {
                            s = -b0 / a00;
                            sqrDistance = b0 * s + c;
                        }
                    }
                } else { // region 1
                    numer = a11 + b1 - a01 - b0;
                    if(numer <= 0.0) {
                        s = 0.0;
                        t = 1.0;
                        sqrDistance = a11 + 2.0 * b1 + c;
                    } else {
                        denom = a00 - 2.0 * a01 + a11;
                        if(numer >= denom) {
                            s = 1.0;
                            t = 0.0;
                            sqrDistance = a00 + 2.0 * b0 + c;
                        } else {
                            s = numer / denom;
                            t = 1.0 - s;
                            sqrDistance = s * (a00 * s + a01 * t + 2.0 * b0) +
                                t * (a01 * s + a11 * t + 2.0 * b1) + c;
                        }
                    }
                }
            }

            // Account for numerical round-off error.
            if(sqrDistance < 0.0) {
                sqrDistance = 0.0;
            }

            closest_point = V0 + s * edge0 + t * edge1;
            lambda0 = 1.0 - s - t;
            lambda1 = s;
            lambda2 = t;
            return sqrDistance;
        }


        template <class VEC>
        inline double point_triangle_squared_distance(
            const VEC& p, const VEC& q1, const VEC& q2, const VEC& q3
        ) {
            VEC closest_point;
            double lambda1, lambda2, lambda3;
            return point_triangle_squared_distance(
                p, q1, q2, q3, closest_point, lambda1, lambda2, lambda3
            );
        }

        inline double tetra_volume_from_edge_lengths(
            double u, double U,
            double v, double V,
            double w, double W
        ) {
            double X = (w - U + v) * (U + v + w);
            double x = (U - v + w) * (v - w + U);
            double Y = (u - V + w) * (V + w + u);
            double y = (V - w + u) * (w - u + V);
            double Z = (v - W + u) * (W + u + v);
            double z = (W - u + v) * (u - v + W);
            double a = ::sqrt(::fabs(x * Y * Z));
            double b = ::sqrt(::fabs(y * Z * X));
            double c = ::sqrt(::fabs(z * X * Y));
            double d = ::sqrt(::fabs(x * y * z));
            return ::sqrt(::fabs(
                    (-a + b + c + d) *
                    (a - b + c + d) *
                    (a + b - c + d) *
                    (a + b + c - d)
                )) / (192.0 * u * v * w);
        }

        template <class VEC>
        inline double tetra_volume(
            const VEC& p1, const VEC& p2, const VEC& p3, const VEC& p4
        ) {
            double U = distance(p1, p2);
            double u = distance(p3, p4);
            double V = distance(p2, p3);
            double v = distance(p1, p4);
            double W = distance(p3, p1);
            double w = distance(p2, p4);
            return tetra_volume_from_edge_lengths(u, U, v, V, w, W);
        }

        template <int DIM>
        inline double tetra_volume(
            const double* p1, const double* p2,
            const double* p3, const double* p4
        ) {
            double U = distance(p1, p2, DIM);
            double u = distance(p3, p4, DIM);
            double V = distance(p2, p3, DIM);
            double v = distance(p1, p4, DIM);
            double W = distance(p3, p1, DIM);
            double w = distance(p2, p4, DIM);
            return tetra_volume_from_edge_lengths(u, U, v, V, w, W);
        }

        template <>
        inline double tetra_volume<3>(
            const double* p1, const double* p2,
            const double* p3, const double* p4
        ) {
            return tetra_volume(
                *reinterpret_cast<const vec3*>(p1),
                *reinterpret_cast<const vec3*>(p2),
                *reinterpret_cast<const vec3*>(p3),
                *reinterpret_cast<const vec3*>(p4)
            );
        }
    }
}

#endif


/******* extracted from ../basic/counted.cpp *******/


namespace GEO {

    Counted::~Counted() {
        geo_assert(nb_refs_ == 0);
    }
}


/******* extracted from ../basic/string.cpp *******/

#include <ctype.h>

namespace GEO {

    static std::string conversion_error(
        const std::string& s, const std::string& type
    ) {
        std::ostringstream out;
        out << "Conversion error: cannot convert string '"
            << s << "' to " << type;
        return out.str();
    }
}

namespace GEO {

    namespace String {

        void split_string(
            const std::string& in,
            char separator,
            std::vector<std::string>& out,
            bool skip_empty_fields
        ) {
            size_t length = in.length();
            size_t start = 0;
            while(start < length) {
                size_t end = in.find(separator, start);
                if(end == std::string::npos) {
                    end = length;
                }
                if(!skip_empty_fields || (end - start > 0)) {
                    out.push_back(in.substr(start, end - start));
                }
                start = end + 1;
            }
        }

        bool GEOGRAM_API split_string(
            const std::string& in,
            char separator,
            std::string& left,
            std::string& right
        ) {
            size_t p = in.find(separator);
            if(p == std::string::npos) {
                left = "";
                right = "";
                return false;
            }
            left = in.substr(0,p);
            right = in.substr(p+1,in.length()-p);
            return true;
        }
        
        std::string join_strings(
            const std::vector<std::string>& in,
            char separator
        ) {
            std::string result;
            for(unsigned int i = 0; i < in.size(); i++) {
                if(result.length() != 0) {
                    result += separator;
                }
                result += in[i];
            }
            return result;
        }

        std::string join_strings(
            const std::vector<std::string>& in,
            const std::string& separator
        ) {
            std::string result;
            for(unsigned int i = 0; i < in.size(); i++) {
                if(result.length() != 0) {
                    result += separator;
                }
                result += in[i];
            }
            return result;
        }

        std::string to_lowercase(const std::string& in) {
            std::string s = in;
            for(unsigned int i = 0; i < s.length(); i++) {
                s[i] = char(tolower(s[i]));
            }
            return s;
        }

        std::string to_uppercase(const std::string& in) {
            std::string s = in;
            for(unsigned int i = 0; i < s.length(); i++) {
                s[i] = char(toupper(s[i]));
            }
            return s;
        }

        std::string quote(const std::string& s, char quotes) {
            return char_to_string(quotes) + s + char_to_string(quotes);
        }

        bool string_starts_with(
            const std::string& haystack, const std::string& needle
        ) {
            return haystack.compare(0, needle.length(), needle) == 0;
        }

        bool string_ends_with(
            const std::string& haystack, const std::string& needle
        ) {
            size_t l1 = haystack.length();
            size_t l2 = needle.length();
            return l1 > l2 && haystack.compare(l1 - l2, l1, needle) == 0;
        }

	// Reference: https://stackoverflow.com/questions/148403/
	//     utf8-to-from-wide-char-conversion-in-stl
	
	std::string wchar_to_UTF8(const wchar_t* in) {
	    std::string out;
	    unsigned int codepoint = 0;
	    for (; *in != 0;  ++in) {
		if (*in >= 0xd800 && *in <= 0xdbff) {
		    codepoint = (unsigned int)(
			((*in - 0xd800) << 10) + 0x10000
		    );
		} else {
		    if (*in >= 0xdc00 && *in <= 0xdfff) {
			codepoint |= (unsigned int)(*in - 0xdc00);
		    } else {
			codepoint = (unsigned int)(*in);
		    }
		
		    if (codepoint <= 0x7f) {
			out.append(1, char(codepoint));
		    } else if (codepoint <= 0x7ff) {
			out.append(1, char(0xc0 | ((codepoint >> 6) & 0x1f)));
			out.append(1, char(0x80 | (codepoint & 0x3f)));
		    } else if (codepoint <= 0xffff) {
			out.append(1, char(0xe0 | ((codepoint >> 12) & 0x0f)));
			out.append(1, char(0x80 | ((codepoint >> 6) & 0x3f)));
			out.append(1, char(0x80 | (codepoint & 0x3f)));
		    } else {
			out.append(1, char(0xf0 | ((codepoint >> 18) & 0x07)));
			out.append(1, char(0x80 | ((codepoint >> 12) & 0x3f)));
			out.append(1, char(0x80 | ((codepoint >> 6) & 0x3f)));
			out.append(1, char(0x80 | (codepoint & 0x3f)));
		    }
		    codepoint = 0;
		}
	    }
	    return out;
	}
	
        

        ConversionError::ConversionError(
            const std::string& s, const std::string& type
        ) :
            std::logic_error(conversion_error(s, type)) {
        }

        const char* ConversionError::what() const GEO_NOEXCEPT {
            return std::logic_error::what();
        }
    }
}


/******* extracted from ../basic/algorithm.h *******/

#ifndef GEOGRAM_BASIC_ALGORITHM
#define GEOGRAM_BASIC_ALGORITHM


#if defined(GEO_OS_LINUX) && defined(GEO_OPENMP)
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4) && !defined(GEO_OS_ANDROID)
#include <parallel/algorithm>
#define GEO_USE_GCC_PARALLEL_STL
#endif
#elif defined(GEO_OS_WINDOWS)
#if (_MSC_VER >= 1700)
#include <ppl.h>
#define GEO_USE_MSVC_PARALLEL_STL
#endif
#endif

#include <algorithm>


namespace GEO {

    bool GEOGRAM_API uses_parallel_algorithm();

    template <typename ITERATOR>
    inline void sort(
        const ITERATOR& begin, const ITERATOR& end
    ) {
        if(uses_parallel_algorithm()) {
#if defined(GEO_USE_GCC_PARALLEL_STL) 
            __gnu_parallel::sort(begin, end);
#elif defined(GEO_USE_MSVC_PARALLEL_STL) 
            concurrency::parallel_sort(begin, end);
#else
            std::sort(begin, end);
#endif
        } else {
            std::sort(begin, end);
        }
    }

    template <typename ITERATOR, typename CMP>
    inline void sort(
        const ITERATOR& begin, const ITERATOR& end, const CMP& cmp
    ) {
        if(uses_parallel_algorithm()) {
#if defined(GEO_USE_GCC_PARALLEL_STL)
            __gnu_parallel::sort(begin, end, cmp);
#elif defined(GEO_USE_MSVC_PARALLEL_STL)
            concurrency::parallel_sort(begin, end, cmp);
#else
            std::sort(begin, end, cmp);
#endif
        } else {
            std::sort(begin, end, cmp);
        }
    }


    template <typename VECTOR> inline void sort_unique(VECTOR& v) {
        std::sort(v.begin(), v.end());
        // Note that std::unique leaves a 'queue' of duplicated elements
        // at the end of the vector, and returns an iterator that
        // indicates where to stop. 
        v.erase(
            std::unique(v.begin(), v.end()), v.end()
        );
    }

    template <typename ITERATOR> inline void sort_3(ITERATOR items) {
	if (items[0]> items[1]) {
	    std::swap(items[0], items[1]);
	}
	if (items[1]> items[2]) {
	    std::swap(items[1], items[2]);
	}
	if (items[0]> items[1]) {
	    std::swap(items[0], items[1]);
	}
    }

    template <typename ITERATOR> inline void sort_4(ITERATOR items) {
	if (items[1] < items[0]) {
	    std::swap(items[0], items[1]);
	}
	if (items[3] < items[2]) {
	    std::swap(items[2], items[3]);
	}
	if (items[2] < items[0]) {
	    std::swap(items[0], items[2]);
	    std::swap(items[1], items[3]);
	}
	if (items[2] < items[1]) {
	    std::swap(items[1], items[2]);
	}
	if (items[3] < items[2]) {
	    std::swap(items[2], items[3]);
	}
    }
    
}

#endif


/******* extracted from ../basic/algorithm.cpp *******/


namespace GEO {

    bool uses_parallel_algorithm() {
        static bool initialized = false;
        static bool result = false;
        if(!initialized) {
            result =
                CmdLine::get_arg_bool("sys:multithread") &&
                CmdLine::get_arg_bool("algo:parallel");
            initialized = true;
        }
        return result;
    }
}


/******* extracted from ../basic/file_system.h *******/

#ifndef GEOGRAM_BASIC_FILE_SYSTEM
#define GEOGRAM_BASIC_FILE_SYSTEM

#include <string>
#include <vector>


namespace GEO {

    namespace FileSystem {

        bool GEOGRAM_API is_file(const std::string& path);

        bool GEOGRAM_API is_directory(const std::string& path);

        bool GEOGRAM_API create_directory(const std::string& path);

        bool GEOGRAM_API delete_directory(const std::string& path);

        bool GEOGRAM_API delete_file(const std::string& path);

        bool GEOGRAM_API get_directory_entries(
            const std::string& path, std::vector<std::string>& result
        );

        std::string GEOGRAM_API get_current_working_directory();

        bool GEOGRAM_API set_current_working_directory(
            const std::string& path
        );

        bool GEOGRAM_API rename_file(
            const std::string& old_name, const std::string& new_name
        );

        Numeric::uint64 GEOGRAM_API get_time_stamp(
            const std::string& path
        );

        std::string GEOGRAM_API extension(const std::string& path);

        std::string GEOGRAM_API base_name(
            const std::string& path, bool remove_extension = true
        );

        std::string GEOGRAM_API dir_name(const std::string& path);

        void GEOGRAM_API get_directory_entries(
            const std::string& path,
            std::vector<std::string>& result, bool recursive
        );

        void GEOGRAM_API get_files(
            const std::string& path,
            std::vector<std::string>& result, bool recursive = false
        );

        void GEOGRAM_API get_subdirectories(
            const std::string& path,
            std::vector<std::string>& result, bool recursive = false
        );

        void GEOGRAM_API flip_slashes(std::string& path);

        bool GEOGRAM_API copy_file(
            const std::string& from, const std::string& to
        );

        void GEOGRAM_API set_executable_flag(const std::string& filename);


        void GEOGRAM_API touch(const std::string& filename);

        std::string GEOGRAM_API normalized_path(const std::string& path);


        std::string GEOGRAM_API home_directory();

        std::string GEOGRAM_API documents_directory();
    }
}

#endif


/******* extracted from ../basic/environment.cpp *******/

#include <algorithm>
#include <stdlib.h>

namespace {
    using namespace GEO;

    class RootEnvironment : public Environment {
    protected:
        
        bool get_local_value(
            const std::string& name, std::string& value
        ) const override {
            auto it = values_.find(name);
            if(it != values_.end()) {
                value = it->second;
                return true;
            }
            return false;
        }

        
        bool set_local_value(
            const std::string& name, const std::string& value
        ) override {
            values_[name] = value;
            return true;
        }

        
        ~RootEnvironment() override {
        }

    private:
        
        typedef std::map<std::string, std::string> ValueMap;
        ValueMap values_;
    };
}

namespace GEO {

    

    VariableObserver::VariableObserver(
        const std::string& var_name
    ) :
        observed_variable_(var_name),
        environment_(nullptr)
    {
        environment_ = Environment::instance()->find_environment(var_name);
        geo_assert(environment_ != nullptr);
        environment_->add_observer(var_name, this);
    }

    VariableObserver::~VariableObserver() {
        environment_->remove_observer(observed_variable_, this);
    }

    

    void VariableObserverList::notify_observers(
        const std::string& value
    ) {
        if(block_notify_) {
            return;
        }
        block_notify_ = true;
        for(size_t i = 0; i < observers_.size(); i++) {
            observers_[i]->value_changed(value);
        }
        block_notify_ = false;
    }

    void VariableObserverList::add_observer(
        VariableObserver* observer
    ) {
	auto it = std::find(observers_.begin(), observers_.end(), observer);
        geo_assert(it == observers_.end());
        observers_.push_back(observer);
    }

    void VariableObserverList::remove_observer(
        VariableObserver* observer
    ) {
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        geo_assert(it != observers_.end());
        observers_.erase(it);
    }

    

    Environment::Environment_var Environment::instance_;

    Environment* Environment::instance() {
        if(instance_ == nullptr) {
            static bool created = false;
            if(created) {
                std::cerr
                    << "CRITICAL: Environment::instance() "
                    << "called after the instance was deleted"
                    << std::endl;
                geo_abort();
            }
            created = true;
            instance_ = new RootEnvironment();
            instance_->add_environment(new SystemEnvironment());
        }
        return instance_;
    }

    void Environment::terminate() {
        instance_.reset();
    }

    Environment::~Environment() {
    }

    bool Environment::add_environment(Environment* env) {
        environments_.push_back(env);
        return true;
    }

    bool Environment::has_value(const std::string& name) const {
        std::string value;
        return get_value(name, value);
    }

    bool Environment::set_value(
        const std::string& name, const std::string& value
    ) {
        for(size_t i = 0; i < environments_.size(); i++) {
            if(environments_[i]->set_value(name, value)) {
                notify_local_observers(name, value);
                return true;
            }
        }
        if(set_local_value(name, value)) {
            notify_local_observers(name, value);
            return true;
        }
        return false;
    }

    bool Environment::get_value(
        const std::string& name, std::string& value
    ) const {
        if(get_local_value(name, value)) {
            return true;
        }
        for(size_t i = 0; i < environments_.size(); i++) {
            if(environments_[i]->get_value(name, value)) {
                return true;
            }
        }
        return false;
    }

    std::string Environment::get_value(const std::string& name) const {
        std::string value;
        bool variable_exists = get_value(name, value);
        if(!variable_exists) {
            Logger::err("Environment")
                << "No such variable: " << name
                << std::endl;
            Logger::err("Environment")
                << "Probably missing CmdLine::import_arg_group(\"...\");"
                << std::endl;
        }
        geo_assert(variable_exists);
        return value;
    }

    Environment* Environment::find_environment(const std::string& name) {
        std::string value;
        if(get_local_value(name, value)) {
            return this;
        }
        for(index_t i=0; i<environments_.size(); ++i) {
            Environment* result = environments_[i]->find_environment(name);
            if(result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }
    
    bool Environment::add_observer(
        const std::string& name, VariableObserver* observer
    ) {
        observers_[name].add_observer(observer);
        return true;
    }

    bool Environment::remove_observer(
        const std::string& name, VariableObserver* observer
    ) {
        auto obs = observers_.find(name);
        geo_assert(obs != observers_.end());
        obs->second.remove_observer(observer);
        return true;
    }

    bool Environment::notify_observers(
        const std::string& name, bool recursive
    ) {
        std::string value = get_value(name);
        return notify_observers(name, value, recursive);
    }

    bool Environment::notify_observers(
        const std::string& name, const std::string& value,
        bool recursive
    ) {
        if(recursive) {
            for(size_t i = 0; i < environments_.size(); i++) {
                environments_[i]->notify_observers(
                    name, value, true
                );
            }
        }
        return notify_local_observers(name, value);
    }

    bool Environment::notify_local_observers(
        const std::string& name, const std::string& value
    ) {
        auto it = observers_.find(name);
        if(it != observers_.end()) {
            it->second.notify_observers(value);
        }
        return true;
    }

    

    SystemEnvironment::~SystemEnvironment() {
    }

    bool SystemEnvironment::set_local_value(
        const std::string& name, const std::string& value
    ) {
        geo_argused(name);
        geo_argused(value);
        return false;
    }

    bool SystemEnvironment::get_local_value(
        const std::string& name, std::string& value
    ) const {
        // For the moment, deactivated under Windows
#ifdef GEO_OS_WINDOWS
        geo_argused(name);
        geo_argused(value);
        return false;
#else
        char* result = ::getenv(name.c_str());
        if(result != nullptr) {
            value = std::string(result);
        }
        return result != nullptr;
#endif
    }
}


/******* extracted from ../basic/command_line.cpp *******/

#include <iostream>
#include <iomanip>

#if defined(GEO_OS_LINUX) || defined(GEO_OS_APPLE)
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#endif

#ifdef GEO_OS_WINDOWS
#include <io.h> // for _isatty()
#endif

#define geo_assert_arg_type(type, allowed_types) \
    geo_assert(((type) & ~(allowed_types)) == 0)

namespace {

    using namespace GEO;
    using namespace CmdLine;

    std::string config_file_name = "geogram.ini";
    
    int geo_argc = 0;
    char** geo_argv = nullptr;
    
    // True if displaying help in a way that
    // it will be easily processed by help2man
    bool man_mode = false;
    
    struct Arg {
        
        std::string name;
        
        std::string desc;
        
        ArgType type;
        
        ArgFlags flags;
    };

    
    typedef std::map<std::string, Arg> Args;

    typedef std::vector<std::string> GroupArgs;

    struct Group {
        
        std::string name;
        
        std::string desc;
        
        ArgFlags flags;
        
        GroupArgs args;
    };

    
    typedef std::map<std::string, Group> Groups;

    
    typedef std::vector<std::string> GroupNames;

    struct CommandLineDesc {
        
        std::string argv0;
        
        Args args;
        
        Groups groups;
        
        GroupNames group_names;
    };

    
    const unsigned int feature_max_length = 12;

    
    CommandLineDesc* desc_ = nullptr;

    bool arg_matches(
        const std::string& arg_in, const std::string& arg_name
    ) {
        return arg_name.find(arg_in) != std::string::npos;
    }

    bool arg_value_error(
        const std::string& name,
        const std::string& s, const char* type
    ) {
        Logger::instance()->set_quiet(false);
        Logger::err("CmdLine")
            << "Argument " << name << " received a bad value: '"
            << s << "' is not a " << type << " value"
            << std::endl;
        return false;
    }

    bool check_arg_value(
        const std::string& name, const std::string& s
    ) {
        ArgType type = get_arg_type(name);
        if(type == ARG_UNDEFINED || type == ARG_STRING) {
            return true;
        }

        if(type == ARG_INT) {
            int value;
            if(!String::from_string(s, value)) {
                return arg_value_error(name, s, "integer");
            } else {
                return true;
            }
        }

        if(type == ARG_DOUBLE) {
            double value;
            if(!String::from_string(s, value)) {
                return arg_value_error(name, s, "floating point");
            } else {
                return true;
            }
        }

        if(type == ARG_BOOL) {
            bool value;
            if(!String::from_string(s, value)) {
                return arg_value_error(name, s, "boolean");
            } else {
                return true;
            }
        }

        if(type == ARG_PERCENT) {
            std::string s2 = s;
            if(s2.length() > 0 && s2[s2.length() - 1] == '%') {
                s2.resize(s2.length() - 1);
            }
            double value;
            if(!String::from_string(s2, value)) {
                return arg_value_error(name, s, "percentage");
            } else {
                return true;
            }
        }

        return false;
    }

    void parse_config_file(int argc, char** argv) {
	geo_assert(argc >= 1);
	std::string program_name = String::to_uppercase(FileSystem::base_name(argv[0]));
	static bool init = false;
	if(init) {
	    return;
	}
	init = true;
	Logger::out("config") << "Configuration file name:" << config_file_name
			      << std::endl;
	Logger::out("config") << "Home directory:" << FileSystem::home_directory()
			      << std::endl;
	std::string config_filename = FileSystem::home_directory() + "/" + config_file_name;
	std::string section = "*";
	if(FileSystem::is_file(config_filename)) {
	    Logger::out("config") << "Using configuration file:"
				       << config_filename
				       << std::endl;
	    std::ifstream in(config_filename.c_str());
	    std::string line;
	    while(std::getline(in,line)) {
		if(line.length() >= 3 && line[0] == '[' && line[line.length()-1] == ']') {
		    section = String::to_uppercase(line.substr(1,line.length()-2));
		} else if(section == program_name || section == "*") {
		    size_t pos = line.find("=");
		    if(pos != std::string::npos) {
			std::string argname = line.substr(0,pos);
			std::string argval  = line.substr(pos+1,line.length()-pos-1);
			if(CmdLine::arg_is_declared(argname)) {
			    CmdLine::set_arg(argname, argval);
			} else {
			    Logger::warn("config") << argname << "=" << argval << " ignored" << std::endl;
			}
		    }
		}
	    }
	}
    }
    
    bool parse_internal(
        int argc, char** argv, std::vector<std::string>& unparsed_args
    ) {
	geo_argc = argc;
	geo_argv = argv;
	
	parse_config_file(argc, argv);
	
        bool ok = true;
        desc_->argv0 = argv[0];
        unparsed_args.clear();

        for(int i = 1; i < argc; i++) {
            std::vector<std::string> parsed_arg;
            String::split_string(argv[i], '=', parsed_arg);
            if(parsed_arg.size() != 2) {
                unparsed_args.push_back(argv[i]);
            } else {
                if(
                    String::string_starts_with(parsed_arg[0], "dbg:") ||
                    desc_->args.find(parsed_arg[0]) != desc_->args.end()
                ) {
                    if(!set_arg(parsed_arg[0], parsed_arg[1])) {
                        ok = false;
                    }
                } else {

                    std::vector<std::string> matches;
                    for( auto& it : desc_->args) {
                        if(arg_matches(parsed_arg[0], it.first)) {
                            matches.push_back(it.first);
                        }
                    }

                    if(matches.size() == 1) {
                        if(!set_arg(matches[0], parsed_arg[1])) {
                            ok = false;
                        }
                    } else if(matches.size() >= 2) {
                        ok = false;
                        Logger::instance()->set_quiet(false);
                        Logger::err("CmdLine")
                            << "Argument is ambiguous: " << argv[i]
                            << std::endl
                            << "Possible matches: "
                            << String::join_strings(matches, ' ')
                            << std::endl;
                    } else {
                        ok = false;
                        Logger::instance()->set_quiet(false);
                        Logger::err("CmdLine")
                            << "Invalid argument: " << parsed_arg[0]
                            << std::endl;
                    }
                }
            }
        }
        return ok;
    }

    std::string arg_group(const std::string& name) {
        size_t pos = name.find(':');
        return pos == std::string::npos
               ? std::string("global")
               : name.substr(0, pos);
    }


    std::string get_display_arg(const std::string& arg_name) {
	CmdLine::ArgType arg_type = get_arg_type(arg_name);
	std::string result;
	if(arg_type == CmdLine::ARG_DOUBLE) {
	    double x = CmdLine::get_arg_double(arg_name);
	    result = String::to_display_string(x);
	} else if(arg_type == CmdLine::ARG_PERCENT) {
	    double x = CmdLine::get_arg_percent(arg_name, 100.0);
	    result = String::to_display_string(x) + "%";
	} else {
	    result = CmdLine::get_arg(arg_name);
	}
	return result;
    }
    
    struct Line {
        std::string name;
        std::string value;
        std::string desc;
    };

    void show_group(const std::string& group, bool advanced) {

        auto it = desc_->groups.find(group);
        if(it == desc_->groups.end()) {
            return;
        }

        const Group& g = it->second;
        bool advanced_group = g.flags & ARG_ADVANCED;
        if(!advanced && advanced_group) {
            return;
        }

        if(advanced_group) {
            ui_separator(g.desc, "*" + g.name);
        } else {
            ui_separator(g.desc, g.name);
        }

        // Step 1:
        // Build a vector of lines that contain the various elements to
        // print for the argument, and compute the maximum width of the
        // argument names followed by their default value. This will allow
        // us to align the closing paren of the default value.

        std::vector<Line> lines;
        index_t max_left_width = 0;

        for(size_t i = 0; i < g.args.size(); i++) {
            auto ita = desc_->args.find(g.args[i]);
            if(ita == desc_->args.end()) {
                continue;
            }

            const Arg& arg = ita->second;
            bool advanced_arg = arg.flags & ARG_ADVANCED;
            if(!advanced && advanced_arg) {
                continue;
            }

            const char* name_marker = (advanced_arg && !man_mode) ? "*" : " ";

            Line line;
            line.name = name_marker + arg.name;
            line.value = " (=" + get_display_arg(arg.name) + ") : ";
            line.desc = arg.desc;
            lines.push_back(line);

            max_left_width = std::max(
                index_t(line.name.length() + line.value.length()),
                max_left_width
            );
        }

        // Step 2:
        // Print the lines constructed in step 1 with default values
        // right aligned.

        for(size_t i = 0; i < lines.size(); i++) {
            const Line& line = lines[i];
            int value_width = int(max_left_width - line.name.length());
            std::ostringstream os;
            os << line.name
                << std::setw(value_width) << line.value
                << line.desc
                << std::endl;
            ui_message(os.str(), max_left_width);
            if(man_mode) {
                ui_message("\n");
            }
        }
    }

}



namespace GEO {

    namespace CmdLine {

        void initialize() {
            desc_ = new CommandLineDesc;
            declare_arg_group("global", "");
        }

        void terminate() {
            ui_close_separator();
            delete desc_;
            desc_ = nullptr;
        }

	int argc() {
	    return geo_argc;
	}

	char** argv() {
	    return geo_argv;
	}

	void set_config_file_name(const std::string& filename) {
	    config_file_name = filename;
	}

	std::string get_config_file_name() {
	    return config_file_name;
	}
	
        bool parse(
            int argc, char** argv, std::vector<std::string>& unparsed_args,
            const std::string& additional_arg_specs
        ) {
            if(!parse_internal(argc, argv, unparsed_args)) {
                return false;
            }

            if(arg_is_declared("profile")) {
                std::string profile = get_arg("profile");
                if(profile != "default") {
                    if(!set_profile(profile)) {
                        return false;
                    }
                    // Re-parse args to override values set by profiles
                    unparsed_args.clear();
                    parse_internal(argc, argv, unparsed_args);
                }
            }

            for(index_t i = 0; i < unparsed_args.size(); ++i) {
                const std::string& arg = unparsed_args[i];
                if(
                    arg == "-h" ||
                    arg == "-?" ||
                    arg == "/h" ||
                    arg == "/?"
                ) {
                    show_usage(additional_arg_specs, true);
                    exit(0);
                }
                if(arg == "--help") {
                    CmdLine::set_arg("log:pretty",false);
                    man_mode = true;
                    show_usage(additional_arg_specs, true);
                    exit(0);
                }
                if(arg == "--version" || arg == "--v") {
                    std::cout << FileSystem::base_name(argv[0])
                     << " "
                     << Environment::instance()->get_value("version")
                     << " (built "
                     << Environment::instance()->get_value(
                         "release_date")
                     << ")"
                     << std::endl
                     << "Copyright (C) 2006-2017"
                     << std::endl
                     << "The Geogram library used by this program is licensed"
                     << std::endl
                     << "under the 3-clauses BSD license."
                     << std::endl
                     << "Inria, the ALICE project"
                     << std::endl
                     << "   <http://alice.loria.fr/software/geogram>"
                     << std::endl
                     << "Report Geogram bugs to the geogram mailing list, see: "
                     << std::endl
                     << "   <https://gforge.inria.fr/mail/?group_id=5833>"
                     << std::endl;
                    exit(0);
                }
            }

            index_t min_unparsed = 0;
            index_t max_unparsed = 0;
            std::vector<std::string> additional_args;
            String::split_string(additional_arg_specs, ' ', additional_args);
            for(index_t i = 0; i < additional_args.size(); ++i) {
                const std::string& arg = additional_args[i];
                if(arg[0] == '<' && arg[arg.length() - 1] == '>') {
                    ++max_unparsed;
                } else if(
                    arg[0] == '<' &&
                    arg[arg.length() - 2] == '>' &&
                    arg[arg.length() - 1] == '*'
                ) {
                    min_unparsed=0;
                    max_unparsed=100000;
                } else {
                    ++max_unparsed;
                    ++min_unparsed;
                }
            }

            if(
                unparsed_args.size() > max_unparsed ||
                unparsed_args.size() < min_unparsed
            ) {
                show_usage(additional_arg_specs);
                return false;
            }

#ifndef GEOGRAM_PSM
	    nlPrintfFuncs(geogram_printf, geogram_fprintf);	    
	    nlInitialize(argc, argv);
#endif
	    if(
		CmdLine::arg_is_declared("nl:CUDA") &&
		CmdLine::get_arg_bool("nl:CUDA")
	    ) {
		geo_cite("DBLP:journals/paapp/BuatoisCL09");
	    }
	    
            return true;
        }

        bool parse(int argc, char** argv) {
            std::vector<std::string> unparsed_args;
            return parse(argc, argv, unparsed_args, "");
        }

        void declare_arg_group(
            const std::string& name,
            const std::string& description,
            ArgFlags flags
        ) {
            if(desc_->groups.find(name) != desc_->groups.end()) {
                Logger::err("CmdLine")
                    << "Group is multiply defined: " << name
                    << std::endl;
                return;
            }

            Group group;
            group.name = name;
            group.desc = description;
            group.flags = flags;
            desc_->groups[name] = group;
            desc_->group_names.push_back(name);
        }

        void declare_arg(
            const std::string& name,
            ArgType type,
            const std::string& default_value,
            const std::string& description,
            ArgFlags flags
        ) {
            if(desc_->args.find(name) != desc_->args.end()) {
                Logger::err("CmdLine")
                    << "Argument is multiply defined: " << name
                    << std::endl;
                return;
            }

            Arg arg;
            arg.name = name;
            arg.type = type;
            arg.desc = description;
            arg.flags = flags;
            desc_->args[name] = arg;

            Environment::instance()->set_value(name, default_value);

            std::string group = arg_group(name);
            auto it = desc_->groups.find(group);
            if(it == desc_->groups.end()) {
                Logger::err("CmdLine")
                    << "Argument group does not exist: " << name
                    << std::endl;
                return;
            }

            it->second.args.push_back(name);
        }

        ArgType get_arg_type(const std::string& name) {
            auto it = desc_->args.find(name);
            return it == desc_->args.end()
                   ? ARG_UNDEFINED
                   : it->second.type;
        }

        std::string get_arg(const std::string& name) {
            return Environment::instance()->get_value(name);
        }

        bool arg_is_declared(const std::string& name) {
            return get_arg_type(name) != ARG_UNDEFINED;
        }

        int get_arg_int(const std::string& name) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_INT);
            return String::to_int(get_arg(name));
        }

        unsigned int get_arg_uint(const std::string& name) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_INT);
            return String::to_uint(get_arg(name));
        }

        double get_arg_double(const std::string& name) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_DOUBLE);
            return String::to_double(get_arg(name));
        }

        double get_arg_percent(
            const std::string& name, double reference
        ) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_PERCENT);
            double result;
            std::string s = get_arg(name);
            if(s.length() > 0 && s[s.length() - 1] == '%') {
                s.resize(s.length() - 1);
                result = String::to_double(s) * reference * 0.01;
                Logger::out("CmdLine")
                    << "using " << name << "=" << result
                    << "(" << get_arg(name) << ")"
                    << std::endl;
            } else {
                result = String::to_double(s);
                Logger::out("CmdLine")
                    << "using " << name << "=" << result
                    << std::endl;
            }
            return result;
        }

        bool get_arg_bool(const std::string& name) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_BOOL);
            return Environment::instance()->has_value(name) &&
                   String::to_bool(get_arg(name));
        }

        bool set_arg(
            const std::string& name, const std::string& value
        ) {
            if(!check_arg_value(name, value)) {
                return false;
            }
            Environment::instance()->set_value(name, value);
            return true;
        }

        void set_arg(const std::string& name, int value) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(
                type, ARG_INT | ARG_DOUBLE | ARG_PERCENT | ARG_STRING
            );
            Environment::instance()->set_value(name, String::to_string(value));
        }

        void set_arg(const std::string& name, unsigned int value) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(
                type, ARG_INT | ARG_DOUBLE | ARG_PERCENT | ARG_STRING
            );
            Environment::instance()->set_value(name, String::to_string(value));
        }

        void set_arg(const std::string& name, double value) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_DOUBLE | ARG_PERCENT | ARG_STRING);
            Environment::instance()->set_value(name, String::to_string(value));
        }

        void set_arg(const std::string& name, bool value) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_BOOL | ARG_STRING);
            Environment::instance()->set_value(name, String::to_string(value));
        }

        void set_arg_percent(const std::string& name, double value) {
            ArgType type = get_arg_type(name);
            geo_assert_arg_type(type, ARG_PERCENT | ARG_STRING);
            Environment::instance()->set_value(
                name, String::to_string(value) + "%"
            );
        }

        void show_usage(const std::string& additional_args, bool advanced) {
            std::string program_name = FileSystem::base_name(desc_->argv0);
            Logger::instance()->set_quiet(false);
            Logger::out("")
                << "Usage: " << program_name << " "
                << additional_args
                << " <parameter=value>*" << std::endl;
            if(!advanced) {
                Logger::out("")
                    << "Showing basic parameters (use " << program_name
                    << " -h to see advanced parameters)"
                    << std::endl;
            }

            for(auto& it : desc_->group_names) {
                show_group(it, advanced);
            }
        }

        void get_args(std::vector<std::string>& args) {
            args.clear();
            for(auto& it : desc_->args) {
                std::string cur_arg = it.first + "=" + get_arg(it.first);
                args.push_back(cur_arg);
            }
        }
    }
}

// =================== Console logging utilies =====================

namespace {

    using namespace GEO;
    using namespace CmdLine;

    
    bool ui_separator_opened = false;

    
    index_t ui_term_width = 79;

    
    index_t ui_left_margin = 0;

    
    index_t ui_right_margin = 0;

    
    const char working[] = {'|', '/', '-', '\\'};

    
    index_t working_index = 0;

    
    const char waves[] = {',', '.', 'o', 'O', '\'', 'O', 'o', '.', ','};

    inline std::ostream& ui_out() {
        return std::cout;
    }

    inline void ui_pad(char c, size_t nb) {
        for(index_t i = 0; i < nb; i++) {
            std::cout << c;
        }
    }

    bool is_redirected() {
        static bool initialized = false;
        static bool result;
        if(!initialized) {
#ifdef GEO_OS_WINDOWS
            result = !_isatty(1);
#else
            result = !isatty(1);
#endif
            initialized = true;
        }
        return result || !Logger::instance()->is_pretty();
    }

    void update_ui_term_width() {
#ifdef GEO_OS_EMSCRIPTEN
        return; // ioctl not implemented under emscripten
#else
#ifndef GEO_OS_WINDOWS
        if(is_redirected()) {
            return;
        }
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        ui_term_width = w.ws_col;
        if(ui_term_width < 20) {
            ui_term_width = 79;
        }
        if(ui_term_width <= 82) {
            ui_left_margin = 0;
            ui_right_margin = 0;
        } else if(ui_term_width < 90) {
            ui_left_margin = 2;
            ui_right_margin = 2;
        } else {
            ui_left_margin = 4;
            ui_right_margin = 4;
        }
#endif
#endif        
    }

    inline size_t sub(size_t a, size_t b) {
        return a > b ? a - b : 0;
    }
}

namespace GEO {

    namespace CmdLine {

        index_t ui_terminal_width() {
            index_t ui_term_width_bkp = ui_term_width;
            update_ui_term_width();
            ui_term_width = std::min(ui_term_width, ui_term_width_bkp);
            return ui_term_width;
        }

        void ui_separator() {
            if(Logger::instance()->is_quiet() || is_redirected()) {
                return;
            }

            update_ui_term_width();
            ui_separator_opened = true;

            ui_out() << " ";
            ui_pad(' ', ui_left_margin);
            ui_pad(
                '_',
                sub(ui_terminal_width(), 2 + ui_left_margin + ui_right_margin)
            );
            ui_out() << " " << std::endl;

            // Force a blank line under the separator
            ui_message("\n");
        }

        void ui_separator(
            const std::string& title,
            const std::string& short_title
        ) {
            if(Logger::instance()->is_quiet()) {
                return;
            }

            if(man_mode) {
                if(title == "") {
                    return;
                }
                ui_out() << std::endl;
                std::string shortt = short_title;
                if(shortt.length() > 0 && shortt[0] == '*') {
                    shortt = shortt.substr(1, shortt.length()-1);
                    ui_out() << title << " (\"" << shortt << ":*\" options, advanced)"
                             << std::endl;
                } else {
                    ui_out() << title << " (\"" << shortt << ":*\" options)"
                             << std::endl;
                }
                ui_out() << std::endl << std::endl;                
                return;
            }
            
            if(is_redirected()) {
                ui_out() << std::endl;
                if(short_title != "" && title != "") {
                    ui_out() << "=[" << short_title << "]=["
                        << title << "]=" << std::endl;
                } else {
                    std::string s = title + short_title;
                    ui_out() << "=[" << s << "]=" << std::endl;
                }
                return;
            }

            update_ui_term_width();
            ui_separator_opened = true;

            size_t L = title.length() + short_title.length();

            ui_out() << "   ";
            ui_pad(' ', ui_left_margin);
            ui_pad('_', L + 14);
            ui_out() << std::endl;

            ui_pad(' ', ui_left_margin);
            if(short_title != "" && title != "") {
                ui_out() << " _/ ==[" << short_title << "]====["
                    << title << "]== \\";
            } else {
                std::string s = title + short_title;
                ui_out() << " _/ =====[" << s << "]===== \\";
            }

            ui_pad(
                '_',
                sub(
                    ui_terminal_width(),
                    19 + L + ui_left_margin + ui_right_margin
                )
            );
            ui_out() << std::endl;

            // Force a blank line under the separator
            ui_message("\n");
        }

        void ui_message(const std::string& message) {
            // By default, wrap to the column that is right after the feature
            // name and its decorations.
            ui_message(message, feature_max_length + 5);
        }

        void ui_message(
            const std::string& message,
            index_t wrap_margin
        ) {
            if(Logger::instance()->is_quiet()) {
                return;
            }

            if(!ui_separator_opened) {
                ui_separator();
            }

            if(is_redirected()) {
                ui_out() << message;
                return;
            }

            std::string cur = message;
            size_t maxL =
                sub(ui_terminal_width(), 4 + ui_left_margin + ui_right_margin);
            index_t wrap = 0;

            for(;;) {
                std::size_t newline = cur.find('\n');
                if(newline != std::string::npos && newline < maxL) {
                    // Got a new line that occurs before the right border
                    // We cut before the newline and pad with spaces
                    ui_pad(' ', ui_left_margin);
                    ui_out() << "| ";
                    ui_pad(' ', wrap);
                    ui_out() << cur.substr(0, newline);
                    ui_pad(' ', maxL - newline);
                    ui_out() << " |" << std::endl;
                    cur = cur.substr(newline + 1);
                }
                else if(cur.length() > maxL) {
                    // The line length runs past the right border
                    // We cut the string just before the border
                    ui_pad(' ', ui_left_margin);
                    ui_out() << "| ";
                    ui_pad(' ', wrap);
                    ui_out() << cur.substr(0, maxL);
                    ui_out() << " |" << std::endl;
                    cur = cur.substr(maxL);
                }
                else if(cur.length() != 0) {
                    // Print the remaining portion of the string
                    // and pad with spaces
                    ui_pad(' ', ui_left_margin);
                    ui_out() << "| ";
                    ui_pad(' ', wrap);
                    ui_out() << cur;
                    ui_pad(' ', maxL - cur.length());
                    ui_out() << " |";
                    break;
                }
                else {
                    // No more chars to print
                    break;
                }

                if(wrap == 0) {
                    wrap = wrap_margin;
                    maxL -= wrap_margin;
                }
            }
        }

        void ui_clear_line() {
            if(Logger::instance()->is_quiet() || is_redirected()) {
                return;
            }

            ui_pad('\b', ui_terminal_width());
            ui_out() << std::flush;
        }

        void ui_close_separator() {
            if(!ui_separator_opened) {
                return;
            }

            if(Logger::instance()->is_quiet() || is_redirected()) {
                return;
            }

            ui_pad(' ', ui_left_margin);
            ui_out() << '\\';
            ui_pad(
                '_',
                sub(ui_terminal_width(), 2 + ui_left_margin + ui_right_margin)
            );
            ui_out() << '/';
            ui_out() << std::endl;

            ui_separator_opened = false;
        }

        void ui_progress(
            const std::string& task_name, index_t val, index_t percent,
            bool clear
        ) {
            if(Logger::instance()->is_quiet() || is_redirected()) {
                return;
            }

            working_index++;

            std::ostringstream os;

            if(percent != val) {
                os << ui_feature(task_name)
                   << "("
                   << working[(working_index % sizeof(working))]
                   << ")-["
                   << std::setw(3) << percent
                   << "%]-["
                   << std::setw(3) << val
                   << "]--[";
            } else {
                os << ui_feature(task_name)
                   << "("
                   << working[(working_index % sizeof(working))]
                   << ")-["
                   << std::setw(3) << percent
                   << "%]--------[";
            }
                
            size_t max_L =
                sub(ui_terminal_width(), 43 + ui_left_margin + ui_right_margin);

            if(val > max_L) {
                // No space enough to expand the progress bar
                // Do some animation...
                for(index_t i = 0; i < max_L; i++) {
                    os << waves[((val - i + working_index) % sizeof(waves))];
                }
            } else {
                for(index_t i = 0; i < val; i++) {
                    os << "o";
                }
            }
            os << " ]";

            if(clear) {
                ui_clear_line();
            }
            ui_message(os.str());
        }

        void ui_progress_time(
            const std::string& task_name, double elapsed, bool clear
        ) {
            if(Logger::instance()->is_quiet()) {
                return;
            }

            std::ostringstream os;
            os << ui_feature(task_name)
                << "Elapsed time: " << elapsed
                << "s\n";

            if(clear) {
                ui_clear_line();
            }
            ui_message(os.str());
        }

        void ui_progress_canceled(
            const std::string& task_name,
            double elapsed, index_t percent, bool clear
        ) {
            if(Logger::instance()->is_quiet()) {
                return;
            }

            std::ostringstream os;
            os << ui_feature(task_name)
                << "Task canceled after " << elapsed
                << "s (" << percent << "%)\n";

            if(clear) {
                ui_clear_line();
            }
            ui_message(os.str());
        }

        std::string ui_feature(
            const std::string& feat_in, bool show
        ) {
            if(feat_in.empty()) {
                return feat_in;
            }

            if(!show) {
                return std::string(feature_max_length + 5, ' ');
            }

            std::string result = feat_in;
            if(!is_redirected()) {
                result = result.substr(0, feature_max_length);
            }
            if(result.length() < feature_max_length) {
                result.append(feature_max_length - result.length(), ' ');
            }
            return "o-[" + result + "] ";
        }
    }
}


/******* extracted from ../basic/command_line_args.cpp *******/


#include <set>

namespace {

    using namespace GEO;
    using namespace CmdLine;

    void import_arg_group_global() {
        declare_arg(
            "profile", "scan",
            "Vorpaline mode "
	    "(scan, convert, repair, heal, cad, tet, poly, hex, quad)"
        );
        declare_arg(
            "debug", false,
            "Toggles debug mode", ARG_ADVANCED
        );
    }

    void import_arg_group_pre() {
        declare_arg_group("pre", "Preprocessing phase");
        declare_arg(
            "pre", true,
            "Toggles pre-processing phase", ARG_ADVANCED
        );
        declare_arg(
            "pre:Nsmooth_iter", 1,
            "Number of iterations for normals smoothing",
            ARG_ADVANCED
        );
        declare_arg_percent(
            "pre:margin", 0,
            "Expand border (in % of bounding box diagonal)",
            ARG_ADVANCED
        );
        declare_arg(
            "pre:repair", false,
            "Repair input mesh"
        );
        declare_arg_percent(
            "pre:epsilon", 0,
            "Colocate tolerance (in % of bounding box diagonal)",
            ARG_ADVANCED
        );  
        declare_arg_percent(
            "pre:max_hole_area", 0,
            "Fill holes smaller than (in % total area)"
        );
        declare_arg(
            "pre:max_hole_edges", 2000,
            "Fill holes with a smaller nb. of edges"
        );
        declare_arg_percent(
            "pre:min_comp_area", 0,
            "Remove small components (in % total area)"
        );
        declare_arg(
            "pre:vcluster_bins", 0,
            "Number of bins for vertex clustering"
        );
        declare_arg(
            "pre:brutal_kill_borders", 0,
            "Brutally kill facets incident to border (nb iter)"
        );
    }

    void import_arg_group_remesh() {
        declare_arg_group("remesh", "Remeshing phase");
        declare_arg(
            "remesh", true,
            "Toggles remeshing phase", ARG_ADVANCED
        );
        declare_arg(
            "remesh:nb_pts", 30000,
            "Number of vertices"
        );
        declare_arg(
            "remesh:anisotropy", 0.0,
            "Anisotropy factor"
        );
        declare_arg(
            "remesh:by_parts", false,
            "Part by part remeshing", ARG_ADVANCED
        );
        declare_arg(
            "remesh:gradation", 0.0,
            "Mesh gradation exponent"
        );
        declare_arg(
            "remesh:lfs_samples", 10000,
            "Number of samples for lfs (gradation)",
            ARG_ADVANCED
        );

#ifdef GEOGRAM_WITH_VORPALINE	
        declare_arg(
            "remesh:sharp_edges", false,
            "Reconstruct sharp edges", ARG_ADVANCED
        );
	
        declare_arg(
            "remesh:Nfactor", 5.0,
            "For sharp_edges", ARG_ADVANCED
        );
#endif
	
        declare_arg(
            "remesh:multi_nerve", true,
            "Insert new vertices to preserve topology",
            ARG_ADVANCED
        );
        declare_arg(
            "remesh:RVC_centroids", true,
            "Use centroids of restricted Voronoi cells", ARG_ADVANCED
        );
        declare_arg(
            "remesh:refine", false,
            "Insert points to lower Hausdorff distance", ARG_ADVANCED
        );
        declare_arg(
            "remesh:max_dist", 0.2,
            "Max. distance to source mesh, relative to avg. edge len",
            ARG_ADVANCED
        );
    }

    void import_arg_group_algo() {
        declare_arg_group("algo", "Algorithms", ARG_ADVANCED);
        declare_arg(
            "algo:nn_search", "BNN",
            "Nearest neighbors search (BNN, ...)"
        );
        declare_arg(
            "algo:delaunay", "NN",
            "Delaunay algorithm"
        );
        declare_arg(
            "algo:hole_filling", "loop_split",
            "Hole filling mode (loop_split, Nloop_split, ear_cut)"
        );
        declare_arg(
            "algo:predicates", "fast",
            "Geometric predicates (fast, exact)"
        );
        declare_arg(
            "algo:reconstruct", "Co3Ne",
            "reconstruction algorithm (Co3Ne, Poisson)"
        );
#ifdef GEO_OS_ANDROID
        // NDK's default multithreading seems to be not SMP-compliant
        // (missing memory barriers in synchronization primitives)
        declare_arg(
            "algo:parallel", false,
            "Use parallel standard algorithms"
        );
#else
        declare_arg(
            "algo:parallel", true,
            "Use parallel standard algorithms"
        );
#endif
    }

    void import_arg_group_post() {
        declare_arg_group("post", "Postprocessing phase");
        declare_arg(
            "post", true,
            "Toggles post-processing phase", ARG_ADVANCED
        );
        declare_arg(
            "post:repair", false,
            "Repair output mesh"
        );
        declare_arg_percent(
            "post:max_hole_area", 0.0,
            "Fill holes smaller than (in % total area)"
        );
        declare_arg(
            "post:max_hole_edges", 2000,
            "Fill holes with a smaller nb. of edges than"
        );
        declare_arg_percent(
            "post:min_comp_area", 0.0,
            "Remove small components (in % total area)"
        );
        declare_arg_percent(
            "post:max_deg3_dist", 0.1,
            "Degree3 vertices threshold (in % bounding box diagonal)"
        );
        declare_arg(
            "post:isect", false, "Tentatively remove self-intersections"
        );
        declare_arg(
            "post:compute_normals", false, "Compute normals"
        );
    }

    void import_arg_group_opt() {
        declare_arg_group("opt", "Optimizer fine tuning", ARG_ADVANCED);
#ifdef GEOGRAM_WITH_HLBFGS
        declare_arg(
            "opt:nb_Lloyd_iter", 5,
            "Number of iterations for Lloyd pre-smoothing"
        );
        declare_arg(
            "opt:nb_Newton_iter", 30,
            "Number of iterations for Newton-CVT"
        );
        declare_arg(
            "opt:nb_LpCVT_iter", 30,
            "Number of iterations for LpCVT"
        );
        declare_arg(
            "opt:Newton_m", 7,
            "Number of evaluations for Hessian approximation"
        );
#else
        declare_arg(
            "opt:nb_Lloyd_iter", 40,
            "Number of iterations for Lloyd pre-smoothing"
        );
        declare_arg(
            "opt:nb_Newton_iter", 0,
            "Number of iterations for Newton-CVT"
        );
        declare_arg(
            "opt:nb_LpCVT_iter", 0,
            "Number of iterations for LpCVT"
        );
        declare_arg(
            "opt:Newton_m", 0,
            "Number of evaluations for Hessian approximation"
        );
#endif	
    }

    void import_arg_group_sys() {
        declare_arg_group("sys", "System fine tuning", ARG_ADVANCED);

#ifdef GEO_DEBUG
        declare_arg(
            "sys:assert", "abort",
            "Assertion behavior (abort, throw, breakpoint)"
        );
#else        
        declare_arg(
            "sys:assert", "throw",
            "Assertion behavior (abort, throw, breakpoint)"
        );
#endif
        
        declare_arg(
            "sys:multithread", Process::multithreading_enabled(),
            "Enables multi-threaded computations"
        );
        declare_arg(
            "sys:FPE", Process::FPE_enabled(),
            "Enables floating-point exceptions"
        );
        declare_arg(
            "sys:cancel", Process::cancel_enabled(),
            "Enables interruption of cancelable tasks"
        );
        declare_arg(
            "sys:max_threads", (int) Process::number_of_cores(),
            "Maximum number of concurrent threads"
        );
        declare_arg(
            "sys:use_doubles", false,
            "Uses double precision in output .mesh files"
        );
        declare_arg(
            "sys:compression_level", 3,
            "Compression level for created .geogram files, in [0..9]"
        );
        declare_arg(
            "sys:lowmem", false,
            "Reduces RAM consumption (but slower)"
        );
        declare_arg(
            "sys:stats", false,
            "Display statistics on exit"
        );
#ifdef GEO_OS_WINDOWS
	declare_arg(
	    "sys:show_win32_console", false,
	    "Display MSDOS window"
	);
#endif	
    }

    void import_arg_group_nl() {
	declare_arg_group("nl", "OpenNL (numerical library)", ARG_ADVANCED);
	declare_arg(
	    "nl:MKL", false,
	    "Use Intel Math Kernel Library (if available in the system)"
	);
	declare_arg(
	    "nl:CUDA", false,
	    "Use NVidia CUDA (if available in the system)"
	);
    }
    
    void import_arg_group_log() {
        declare_arg_group("log", "Logger settings", ARG_ADVANCED);
        declare_arg(
            "log:quiet", false,
            "Turns logging on/off"
        );
        declare_arg(
            "log:pretty", true,
            "Turns console pretty output on/off"
        );
        declare_arg(
            "log:file_name", "",
            "Logs output to the specified file"
        );
        declare_arg(
            "log:features", "*",
            "Semicolon separated list of features selected for log"
        );
        declare_arg(
            "log:features_exclude", "",
            "Semicolon separated list of features excluded from log"
        );
    }

    void import_arg_group_co3ne() {
        declare_arg_group("co3ne", "Reconstruction", ARG_ADVANCED);
        declare_arg(
            "co3ne", false,
            "Use reconstruction", ARG_ADVANCED
        );
        declare_arg(
            "co3ne:nb_neighbors", 30,
            "Number of neighbors used in reconstruction"
        );
        declare_arg(
            "co3ne:Psmooth_iter", 0, "Number of smoothing iterations"
        );
        declare_arg_percent(
            "co3ne:radius", 5,
            "Search radius (in % bounding box diagonal)"
        );
        declare_arg(
            "co3ne:repair", true,
            "Repair output surface"
        );
        declare_arg(
            "co3ne:max_N_angle", 60.0,
            "Filter bad triangles (in degrees)"
        );
        declare_arg_percent(
            "co3ne:max_hole_area", 5,
            "Fill holes smaller than (in % total area)"
        );
        declare_arg(
            "co3ne:max_hole_edges", 500,
            "Fill holes with a smaller nb. of edges"
        );
        declare_arg_percent(
            "co3ne:min_comp_area", 0.01,
            "Remove small components (in % total area)"
        );
        declare_arg(
            "co3ne:min_comp_facets", 10,
            "Remove small components (in facet nb.)"
        );
        declare_arg(
            "co3ne:T12", true,
            "Use also triangles seen from 1 and 2 seeds"
        );
        declare_arg(
           "co3ne:strict", false,
           "enforce combinatorial tests for triangles seen from 3 seeds as well"
        );
        declare_arg(
            "co3ne:use_normals", true,
            "Use existing normal attached to data if available"
        );

        // For now, in co3ne import arg group -> todo: create new import func
        declare_arg_group("poisson", "Reconstruction", ARG_ADVANCED);
        declare_arg(
            "poisson:octree_depth", 8,
            "Octree depth for Poisson reconstruction if used"
        );
    }

    void import_arg_group_stat() {
        declare_arg_group("stat", "Statistics tuning");
        declare_arg_percent(
            "stat:sampling_step", 0.5,
            "For Hausdorff distance"
        );
    }

    void import_arg_group_poly() {
        declare_arg_group("poly", "Polyhedral meshing", ARG_ADVANCED);
        declare_arg(
            "poly", false,
            "Toggles polyhedral meshing"
        );
	declare_arg(
	    "poly:simplify", "tets_voronoi",
	    "one of none (generate all intersections), "
	    "tets (regroup Vornoi cells), "
	    "tets_voronoi (one polygon per Voronoi facet), "
	    "tets_voronoi_boundary (simplify boundary)"
	);
	declare_arg(
	    "poly:normal_angle_threshold", 1e-3,
	    "maximum normal angle deviation (in degrees) for merging boundary facets"
	    " (used if poly:simplify=tets_voronoi_boundary)"
	);
	declare_arg(
	    "poly:cells_shrink", 0.0,
	    "Voronoi cells shrink factor (for visualization purposes), between 0.0 and 1.0"
	);
	declare_arg(
	    "poly:points_file", "",
	    "optional points file name (if left blank, generates and optimizes remesh:nb_pts points)"
	);
	declare_arg(
	    "poly:generate_ids", false,
	    "generate unique ids for vertices and cells (saved in geogram, geogram_ascii and ovm file formats only)"
	);
	declare_arg(
	    "poly:embedding_dim", 0,
	    "force embedding dimension (0 = use input dim.)"
	);
	declare_arg(
	    "poly:tessellate_non_convex_facets", false,
	    "tessellate non-convex facets"
	);
    }    

    void import_arg_group_hex() {
        declare_arg_group("hex", "Hex-dominant meshing", ARG_ADVANCED);
        declare_arg(
            "hex", false,
            "Toggles hex-dominant meshing"
        );
        declare_arg(
            "hex:save_points", false,
            "Save points to points.meshb"
        );
        declare_arg(
            "hex:save_tets", false,
            "Save tetrahedra (before primitive merging) to tets.meshb"
        );
        declare_arg(
            "hex:save_surface", false,
            "Save surface to surface.meshb"
        );
        declare_arg(
            "hex:save_frames", false,
            "Save frames and surface to frames_surface.eobj"
        );
        declare_arg(
            "hex:prefer_seeds", true,
            "In constrained mode, use seeds whenever possible"
        );
        declare_arg(
            "hex:constrained", true,
            "Use constrained Delaunay triangulation"
        );
        declare_arg(
            "hex:points_file", "",
            "Load points from a file"
        );
        declare_arg(
            "hex:tets_file", "",
            "Load tetrahedra from a file"
        );
        declare_arg(
            "hex:frames_file", "",
            "Load frames from a file"
        );
        declare_arg(
            "hex:prisms", false,
            "generate prisms"
        );
        declare_arg(
            "hex:pyramids", false,
            "generate pyramids"
        );
        declare_arg(
            "hex:algo", "PGP3d",
            "one of (PGP3d, LpCVT)"
        );
        declare_arg(
            "hex:PGP_max_corr_prop", 0.35,
            "maximum correction form (0 to deactivate)"
        );
        declare_arg(
            "hex:PGP_FF_free_topo", 1,
            "number of free topo. frame field opt. iterations"
        );
        declare_arg(
            "hex:PGP_FF_fixed_topo", 1,
            "number of fixed topo. frame field opt. iterations"
        );
        declare_arg(
            "hex:PGP_direct_solver", false,
            "(tentatively) use PGP direct solver"
        );
        declare_arg(
            "hex:border_refine", false,
            "refine border to lower Hausdorff distance"
        );
        declare_arg_percent(
            "hex:border_max_distance", 20,
            "maximum distance to reference (in % of input average edge length)"
        );
        declare_arg(
            "hex:keep_border", false, "keep initial border mesh"
        );
    }

    void import_arg_group_quad() {
        declare_arg_group("quad", "Quad-dominant meshing", ARG_ADVANCED);
        declare_arg(
            "quad", false,
            "Toggles quad-dominant meshing"
        );
	declare_arg(
	    "quad:relative_edge_length",
	    1.0,
	    "relative edge length"
	);
	declare_arg(
	    "quad:optimize_parity", false,
	    "Optimize quads parity when splitting charts (experimental)"
	);
	declare_arg(
	    "quad:max_scaling_correction", 1.0,
	    "maximum scaling correction factor (use 1.0 to disable)"
	);
    }
    
    void import_arg_group_tet() {
        declare_arg_group("tet", "Tetrahedral meshing", ARG_ADVANCED);
        declare_arg(
            "tet", false,
            "Toggles tetrahedral meshing"
        );
        declare_arg(
            "tet:refine", true,
            "Generates additional points to improve mesh quality"
        );
        declare_arg(
            "tet:preprocess", true,
            "Pre-processes surface before meshing"
        );
        declare_arg(
            "tet:quality", 2.0,
        "desired element quality (the lower, the better, 2.0 means reasonable)"
        );
    }

    void import_arg_group_gfx() {
        declare_arg_group("gfx", "OpenGL graphics options", ARG_ADVANCED);


// Default profile will be "core" in a short future for all architectures,
// but some users reported problems with it, so I keep for now
// "compatibility" as the default (except on Mac/OS that prefers "core")	
        declare_arg(
            "gfx:GL_profile",
#if defined(GEO_OS_APPLE)
	    "core",
#elif defined(GEO_OS_ANDROID)
	    "ES",	    
#else
	    "compatibility",	    
#endif	    
            "one of core,compatibility,ES"
        );
        declare_arg(
            "gfx:GL_version", 0.0,
            "If non-zero, override GL version detection"
        );
        declare_arg(
            "gfx:GL_debug", false,
            "OpenGL debugging context"
        );
        declare_arg(
            "gfx:GLSL", true,
            "Use GLSL shaders (requires a decently recent gfx board)"
        );
        declare_arg(
            "gfx:GLSL_version", 0.0,
            "If non-zero, overrides GLSL version detection"
        );
        declare_arg(
            "gfx:GLUP_profile", "auto",
            "one of auto, GLUP150, GLUP440, VanillaGL"
        );
        declare_arg("gfx:full_screen", false, "full screen mode");
        declare_arg(
	    "gfx:no_decoration", false,
	    "no window decoration (full screen mode)"
	);	
	declare_arg(
	    "gfx:transparent", false,
	    "use transparent backgroung (desktop integration)"
	);
        declare_arg(
            "gfx:GLSL_tesselation", true, "use tesselation shaders if available"
        );
	declare_arg("gfx:geometry", "800x800", "resolution");
    }
    
    void import_arg_group_biblio() {
        declare_arg_group("biblio", "Bibliography options", ARG_ADVANCED);
	declare_arg("biblio", false, "output bibliography citations");
	declare_arg(
	    "biblio:command_line", false,
	    "dump all command line arguments in biblio. report"
	);
    }
    
    

    void set_profile_cad() {
        set_arg("pre:repair", true);
        set_arg_percent("pre:margin", 0.05);
        set_arg("post:repair", true);
        set_arg("remesh:sharp_edges", true);
        set_arg("remesh:RVC_centroids", false);
    }

    void set_profile_scan() {
        set_arg("pre:Nsmooth_iter", 3);
        set_arg("pre:repair", true);
        set_arg_percent("pre:max_hole_area", 10);
        set_arg("remesh:anisotropy", 1.0);
        set_arg_percent("pre:min_comp_area", 3);
        set_arg_percent("post:min_comp_area", 3);
    }

    void set_profile_convert() {
        set_arg("pre", false);
        set_arg("post", false);
        set_arg("remesh", false);
    }

    void set_profile_repair() {
        set_arg("pre", true);
        set_arg("pre:repair", true);
        set_arg("post", false);
        set_arg("remesh", false);
    }

    void set_profile_heal() {
        set_arg("remesh", true);
        set_arg("remesh:multi_nerve", false);
        set_arg("post", true);
        set_arg_percent("post:max_hole_area", 10);
        set_arg_percent("post:min_comp_area", 3);
    }

    void set_profile_reconstruct() {
        set_arg("pre", false);
        set_arg("post", false);
        set_arg("remesh", false);
        set_arg("co3ne", true);
    }

    void set_profile_hex() {
        set_arg("hex", true);
    }

    void set_profile_quad() {
        set_arg("quad", true);
    }
    
    void set_profile_tet() {
        set_arg("tet", true);
    }

    void set_profile_poly() {
        set_arg("poly", true);
    }
}

namespace GEO {

    namespace CmdLine {

        bool import_arg_group(
            const std::string& name
        ) {
	    static std::set<std::string> imported;
	    if(imported.find(name) != imported.end()) {
		return true;
	    }
	    imported.insert(name);
	    
            if(name == "standard") {
                import_arg_group_global();
                import_arg_group_sys();
		import_arg_group_nl();		
                import_arg_group_log();
		import_arg_group_biblio();
            } else if(name == "global") {
                import_arg_group_global();
            } else if(name == "nl") {
	        import_arg_group_nl();
	    } else if(name == "sys") {
                import_arg_group_sys();
            } else if(name == "log") {
                import_arg_group_log();
            } else if(name == "pre") {
                import_arg_group_pre();
            } else if(name == "remesh") {
                import_arg_group_remesh();
            } else if(name == "algo") {
                import_arg_group_algo();
            } else if(name == "post") {
                import_arg_group_post();
            } else if(name == "opt") {
                import_arg_group_opt();
            } else if(name == "co3ne") {
                import_arg_group_co3ne();
            } else if(name == "stat") {
                import_arg_group_stat();
            } else if(name == "quad") {
                import_arg_group_quad();
            } else if(name == "hex") {
                import_arg_group_hex();
            } else if(name == "tet") {
                import_arg_group_tet();
            } else if(name == "poly") {
                import_arg_group_poly();
            } else if(name == "gfx") {
                import_arg_group_gfx();
            } else {
                Logger::instance()->set_quiet(false);
                Logger::err("CmdLine")
                    << "No such option group: " << name
                    << std::endl;
                return false;
            }
            return true;
        }

        bool set_profile(
            const std::string& name
        ) {
            if(name == "cad") {
                set_profile_cad();
            } else if(name == "scan") {
                set_profile_scan();
            } else if(name == "convert") {
                set_profile_convert();
            } else if(name == "repair") {
                set_profile_repair();
            } else if(name == "heal") {
                set_profile_heal();
            } else if(name == "reconstruct") {
                set_profile_reconstruct();
            } else if(name == "tet") {
                set_profile_tet();
            } else if(name == "quad") {
                set_profile_quad();
            } else if(name == "hex") {
                set_profile_hex();
            } else if(name == "poly") {
                set_profile_poly();
            } else {
                Logger::instance()->set_quiet(false);
                Logger::err("CmdLine")
                    << "No such profile: " << name
                    << std::endl;
                return false;
            }
            return true;
        }
    }
}


/******* extracted from ../basic/line_stream.cpp *******/

#include <ctype.h>

namespace GEO {

    LineInput::LineInput(const std::string& filename) :
        file_name_(filename),
        line_num_(0)
    {
        F_ = fopen(filename.c_str(), "r");
        ok_ = (F_ != nullptr);
        line_[0] = '\0';
    }

    LineInput::~LineInput() {
        if(F_ != nullptr) {
            fclose(F_);
            F_ = nullptr;
        }
    }

    bool LineInput::get_line() {
        if(F_ == nullptr) {
            return false;
        }
        line_[0] = '\0';
        // Skip the empty lines
        while(!isprint(line_[0])) {
            ++line_num_;
            if(fgets(line_, MAX_LINE_LEN, F_) == nullptr) {
                return false;
            }
        }
        // If the line ends with a backslash, append
        // the next line to the current line.
        bool check_multiline = true;
        Numeric::int64 total_length = MAX_LINE_LEN;
        char* ptr = line_;
        while(check_multiline) {
            size_t L = strlen(ptr);
            total_length -= Numeric::int64(L);
            ptr = ptr + L - 2;
            if(*ptr == '\\' && total_length > 0) {
                *ptr = ' ';
                ptr++;
                if(fgets(ptr, int(total_length), F_) == nullptr) {
                    return false;
                }
                ++line_num_;
            } else {
                check_multiline = false;
            }
        }
        if(total_length < 0) {
            Logger::err("LineInput")
                << "MultiLine longer than "
                << MAX_LINE_LEN << " bytes" << std::endl;
        }
        return true;
    }

#if 1
#ifdef GEO_OS_WINDOWS
#define safe_strtok strtok_s
#else
#define safe_strtok strtok_r
#endif

    void LineInput::get_fields(const char* separators) {
        field_.resize(0);
        char* context = nullptr;
        char* tok = safe_strtok(line_, separators, &context);
        while(tok != nullptr) {
            field_.push_back(tok);
            tok = safe_strtok(nullptr, separators, &context);
        }
    }

#else
    void LineInput::get_fields(const char* separators) {
        field_.resize(0);
        char* tok = strtok(line_, separators);
        while(tok != nullptr) {
            field_.push_back(tok);
            tok = strtok(nullptr, separators);
        }
    }

#endif

    void LineInput::conversion_error(index_t index, const char* type) const {
        std::ostringstream out;
        out << "Line " << line_num_
            << ": field #" << index
            << " is not a valid " << type << " value: " << field(index);
        throw std::logic_error(out.str());
    }
}


/******* extracted from ../basic/logger.cpp *******/


#include <stdlib.h>
#include <stdarg.h>

/*
   Disables the warning caused by passing 'this' as an argument while
   construction is not finished (in LoggerStream ctor).
   As LoggerStreamBuf only stores the pointer for later use, so we can
   ignore the fact that 'this' is not completely formed yet.
 */
#ifdef GEO_OS_WINDOWS
#pragma warning(disable:4355)
#endif

namespace GEO {

    

    int LoggerStreamBuf::sync() {
        std::string str(this->str());
        loggerStream_->notify(str);
        this->str("");
        return 0;
    }

    

    LoggerStream::LoggerStream(Logger* logger) :
        std::ostream(new LoggerStreamBuf(this)),
        logger_(logger) {
    }

    LoggerStream::~LoggerStream() {
        std::streambuf* buf = rdbuf();
        delete buf;
    }

    void LoggerStream::notify(const std::string& str) {
	logger_->notify(this, str);
    }

    

    LoggerClient::~LoggerClient() {
    }

    

    ConsoleLogger::ConsoleLogger() {
    }

    ConsoleLogger::~ConsoleLogger() {
    }

    void ConsoleLogger::div(const std::string& title) {
        CmdLine::ui_separator(title);
    }

    void ConsoleLogger::out(const std::string& str) {
        CmdLine::ui_message(str);
    }

    void ConsoleLogger::warn(const std::string& str) {
        CmdLine::ui_message(str);
    }

    void ConsoleLogger::err(const std::string& str) {
        CmdLine::ui_message(str);
    }

    void ConsoleLogger::status(const std::string& str) {
        geo_argused(str);
    }

    

    FileLogger::FileLogger() :
        log_file_(nullptr) {
    }

    FileLogger::FileLogger(const std::string& file_name) :
        log_file_(nullptr)
    {
        set_file_name(file_name);
    }

    FileLogger::~FileLogger() {
        delete log_file_;
        log_file_ = nullptr;
    }

    void FileLogger::set_file_name(const std::string& file_name) {
        log_file_name_ = file_name;
        if(log_file_ != nullptr) {
            delete log_file_;
            log_file_ = nullptr;
        }
        if(log_file_name_.length() != 0) {
            log_file_ = new std::ofstream(log_file_name_.c_str());
        }
    }

    void FileLogger::div(const std::string& title) {
        if(log_file_ != nullptr) {
            *log_file_
                << "\n====[" << title << "]====\n"
                << std::flush;
        }
    }

    void FileLogger::out(const std::string& str) {
        if(log_file_ != nullptr) {
            *log_file_ << str << std::flush;
        }
    }

    void FileLogger::warn(const std::string& str) {
        if(log_file_ != nullptr) {
            *log_file_ << str << std::flush;
        }
    }

    void FileLogger::err(const std::string& str) {
        if(log_file_ != nullptr) {
            *log_file_ << str << std::flush;
        }
    }

    void FileLogger::status(const std::string& str) {
        geo_argused(str);
    }

    

    SmartPointer<Logger> Logger::instance_;

    void Logger::initialize() {
        instance_ = new Logger();
        Environment::instance()->add_environment(instance_);
    }

    void Logger::terminate() {
        instance_.reset();
    }

    bool Logger::is_initialized() {
        return (instance_ != nullptr);
    }
    
    bool Logger::set_local_value(
        const std::string& name, const std::string& value
    ) {

        if(name == "log:quiet") {
            set_quiet(String::to_bool(value));
            return true;
        }

        if(name == "log:minimal") {
            set_minimal(String::to_bool(value));
            return true;
        }
        
        if(name == "log:pretty") {
            set_pretty(String::to_bool(value));
            return true;
        }

        if(name == "log:file_name") {
            log_file_name_ = value;
            if(!log_file_name_.empty()) {
                register_client(new FileLogger(log_file_name_));
            }
            return true;
        }

        if(name == "log:features") {
            std::vector<std::string> features;
            String::split_string(value, ';', features);
            log_features_.clear();
            if(features.size() == 1 && features[0] == "*") {
                log_everything_ = true;
            } else {
                log_everything_ = false;
                for(size_t i = 0; i < features.size(); i++) {
                    log_features_.insert(features[i]);
                }
            }
            notify_observers(name);
            return true;
        }

        if(name == "log:features_exclude") {
            std::vector<std::string> features;
            String::split_string(value, ';', features);
            log_features_exclude_.clear();
            for(size_t i = 0; i < features.size(); i++) {
                log_features_exclude_.insert(features[i]);
            }
            notify_observers(name);
            return true;
        }

        return false;
    }

    bool Logger::get_local_value(
        const std::string& name, std::string& value
    ) const {

        if(name == "log:quiet") {
            value = String::to_string(is_quiet());
            return true;
        }

        if(name == "log:minimal") {
            value = String::to_string(is_minimal());
            return true;
        }
        
        if(name == "log:pretty") {
            value = String::to_string(is_pretty());
            return true;
        }

        if(name == "log:file_name") {
            value = log_file_name_;
            return true;
        }

        if(name == "log:features") {
            if(log_everything_) {
                value = "*";
            } else {
                value = "";
                for(auto& it : log_features_) {
                    if(value.length() != 0) {
                        value += ';';
                    }
                    value += it;
                }
            }
            return true;
        }

        if(name == "log:features_exclude") {
            value = "";
            for(auto& it : log_features_exclude_) {
                if(value.length() != 0) {
                    value += ';';
                }
                value += it;
            }
            return true;
        }

        return false;
    }

    void Logger::register_client(LoggerClient* c) {
        clients_.insert(c);
    }

    void Logger::unregister_client(LoggerClient* c) {
        geo_debug_assert(clients_.find(c) != clients_.end());
	clients_.erase(c);
    }

    void Logger::unregister_all_clients() {
        clients_.clear();
    }

    bool Logger::is_client(LoggerClient* c) const {
        return clients_.find(c) != clients_.end();
    }

    void Logger::set_quiet(bool flag) {
        quiet_ = flag;
    }

    void Logger::set_minimal(bool flag) {
        minimal_ = flag;
    }
    
    void Logger::set_pretty(bool flag) {
        pretty_ = flag;
    }


    Logger::Logger() :
        out_(this),
        warn_(this),
        err_(this),
        status_(this),
        log_everything_(true),
        current_feature_changed_(false),
        quiet_(true),
        pretty_(true),
        minimal_(false),
	notifying_error_(false)
    {
        // Add a default client printing stuff to std::cout
        register_client(new ConsoleLogger());
#ifdef GEO_DEBUG
        quiet_ = false;
#endif        
    }

    Logger::~Logger() {
    }

    Logger* Logger::instance() {
        // Do not use geo_assert here:
	//  if the instance is nullptr, geo_assert will
        // call the Logger to print the assertion failure, thus ending in a
        // infinite loop.
        if(instance_ == nullptr) {
            std::cerr
                << "CRITICAL: Accessing uninitialized Logger instance"
                << std::endl;
            geo_abort();
        }
        return instance_;
    }

    std::ostream& Logger::div(const std::string& title) {
	std::ostream& result = 
   	    (is_initialized() && !Process::is_running_threads()) ?
            instance()->div_stream(title) :
            (std::cerr << "=====" << title << std::endl);
	return result;
    }

    std::ostream& Logger::out(const std::string& feature) {
	std::ostream& result =
	    (is_initialized() && !Process::is_running_threads()) ?
            instance()->out_stream(feature) :
            (std::cerr << "    [" << feature << "] ");
	return result;
    }

    std::ostream& Logger::err(const std::string& feature) {
	std::ostream& result = 
	    (is_initialized() && !Process::is_running_threads()) ?	    
            instance()->err_stream(feature) :
            (std::cerr << "(E)-[" << feature << "] ");
	return result;
    }

    std::ostream& Logger::warn(const std::string& feature) {
	std::ostream& result = 
	    (is_initialized() && !Process::is_running_threads()) ?	    	    
            instance()->warn_stream(feature) :
            (std::cerr << "(W)-[" << feature << "] ");
	return result;
    }

    std::ostream& Logger::status() {
	std::ostream& result =	
	    (is_initialized() && !Process::is_running_threads()) ?	    	    	
            instance()->status_stream() :
            (std::cerr << "[status] ");
	return result;
    }

    std::ostream& Logger::div_stream(const std::string& title) {
        if(!quiet_) {
            current_feature_changed_ = true;
            current_feature_.clear();
            for(auto it : clients_) {
                it->div(title);
            }
        }
        return out_;
    }

    std::ostream& Logger::out_stream(const std::string& feature) {
        if(!quiet_ && !minimal_ && current_feature_ != feature) {
            current_feature_changed_ = true;
            current_feature_ = feature;
        }
        return out_;
    }

    std::ostream& Logger::err_stream(const std::string& feature) {
        if(!quiet_ && current_feature_ != feature) {
            current_feature_changed_ = true;
            current_feature_ = feature;
        }
        return err_;
    }

    std::ostream& Logger::warn_stream(const std::string& feature) {
        if(!quiet_ && current_feature_ != feature) {
            current_feature_changed_ = true;
            current_feature_ = feature;
        }
        return warn_;
    }

    std::ostream& Logger::status_stream() {
        return status_;
    }

    void Logger::notify_out(const std::string& message) {
        if(
            (log_everything_ &&
                log_features_exclude_.find(current_feature_) ==
                log_features_exclude_.end())
            || (log_features_.find(current_feature_) != log_features_.end())
        ) {
            std::string feat_msg =
                CmdLine::ui_feature(current_feature_, current_feature_changed_)
                + message;

	    for(auto it : clients_) {
                it->out(feat_msg);
            }

            current_feature_changed_ = false;
        }
    }

    void Logger::notify_warn(const std::string& message) {
        std::string msg = "Warning: " + message;
        std::string feat_msg =
            CmdLine::ui_feature(current_feature_, current_feature_changed_)
            + msg;

        for(auto it : clients_) {
            it->warn(feat_msg);
            it->status(msg);
        }

        current_feature_changed_ = false;
    }

    void Logger::notify_err(const std::string& message) {
        std::string msg = "Error: " + message;
        std::string feat_msg =
            CmdLine::ui_feature(current_feature_, current_feature_changed_)
            + msg;

	if(notifying_error_) {
	    std::cerr << "Error while displaying error (!):"
		      << feat_msg << std::endl;
	} else {
	    notifying_error_ = true;
	    for(auto it : clients_) {
		it->err(feat_msg);
		it->status(msg);
	    }
	    notifying_error_ = false;
	}

        current_feature_changed_ = false;
    }

    void Logger::notify_status(const std::string& message) {
        for(auto it : clients_) {
            it->status(message);
        }

        current_feature_changed_ = false;
    }

    void Logger::notify(LoggerStream* s, const std::string& message) {

        if(quiet_ || (minimal_ && s == &out_) || clients_.empty()) {
            return;
        }

        if(s == &out_) {
            notify_out(message);
        } else if(s == &warn_) {
            notify_warn(message);
        } else if(s == &err_) {
            notify_err(message);
        } else if(s == &status_) {
            notify_status(message);
        } else {
            geo_assert_not_reached;
        }
    }

    
    
}

extern "C" {

    int geogram_printf(const char* format, ...) {

        static std::string last_string;

        va_list args;

        // Get the number of characters to be printed.        
        va_start(args, format);
        int nb = vsnprintf(nullptr, 0, format, args)+1; // +1, I don't know why...
        va_end(args);

        // Generate the output string
        GEO::vector<char> buffer(GEO::index_t(nb+1));
        va_start(args, format);
        vsnprintf(buffer.data(),buffer.size()-1, format, args);
        va_end(args);

        // Find the lines in the generated string
        GEO::vector<char*> lines;
        lines.push_back(buffer.data());
        char last_char = '\n';
        for(char* ptr = buffer.data(); *ptr; ptr++) {
            if(*ptr != '\0') {
                last_char = *ptr;
            }
            if(*ptr == '\n') {
                *ptr = '\0';
                ptr++;
                if(*ptr != '\0') {
                    lines.push_back(ptr);
                }
            }
        }

        // If last character is not a carriage return,
        // memorize the last line for later.
        if(last_char != '\n') {
            last_string += *lines.rbegin();
            lines.pop_back();
        }

        // Output all the lines.
        // Prepend the optionally memorized previous strings to the
        // first one.
        for(GEO::index_t i=0; i<lines.size(); ++i) {
            if(i == 0) {
                GEO::Logger::out("") << last_string << lines[i] << std::endl;
                last_string.clear();
            } else {
                GEO::Logger::out("") << lines[i] << std::endl;                
            }
        }

	return nb;
    }

    int geogram_fprintf(FILE* out, const char* format, ...) {


        static std::string last_string;

        va_list args;

        // Get the number of characters to be printed.        
        va_start(args, format);
        int nb = vsnprintf(nullptr, 0, format, args)+1; // +1, I don't know why...
        va_end(args);

        // Generate the output string
        GEO::vector<char> buffer(GEO::index_t(nb+1));
        va_start(args, format);
        vsnprintf(buffer.data(),buffer.size()-1, format, args);
        va_end(args);

        // Find the lines in the generated string
        GEO::vector<char*> lines;
        lines.push_back(buffer.data());
        char last_char = '\n';
        for(char* ptr = buffer.data(); *ptr; ptr++) {
            if(*ptr != '\0') {
                last_char = *ptr;
            }
            if(*ptr == '\n') {
                *ptr = '\0';
                ptr++;
                if(*ptr != '\0') {
                    lines.push_back(ptr);
                }
            }
        }

        // If last character is not a carriage return,
        // memorize the last line for later.
        if(last_char != '\n') {
            last_string += *lines.rbegin();
            lines.pop_back();
        }

        // Output all the lines.
        // Prepend the optionally memorized previous strings to the
        // first one.
        for(GEO::index_t i=0; i<lines.size(); ++i) {
            if(i == 0) {
                if(out == stdout) {
                    GEO::Logger::out("") << last_string << lines[i] << std::endl;
                } else if(out == stderr) {
                    GEO::Logger::err("") << last_string << lines[i] << std::endl;                    
                } else {
                    fprintf(out, "%s%s", last_string.c_str(), lines[i]);                    
                }
                last_string.clear();
            } else {
                if(out == stdout) {
                    GEO::Logger::out("") << lines[i] << std::endl;
                } else if(out == stderr) {
                    GEO::Logger::err("") << lines[i] << std::endl;                    
                } else {
                    fprintf(out, "%s", lines[i]);                    
                }
            }
        }
	
	return nb;
    }
}


/******* extracted from ../basic/file_system.cpp *******/


#include <iostream>
#include <fstream>
#include <assert.h>

#ifdef GEO_OS_WINDOWS
#include <windows.h>
#include <io.h>
#include <shlobj.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#endif

namespace GEO {

    namespace FileSystem {

        // OS-dependent functions
#ifdef GEO_OS_WINDOWS

        bool is_file(const std::string& path) {
            WIN32_FIND_DATA file;
            HANDLE file_handle = FindFirstFile(path.c_str(), &file);
            if(file_handle == INVALID_HANDLE_VALUE) {
                return false;
            }
            FindClose(file_handle);
            return (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
        }

        bool is_directory(const std::string& path) {
            WIN32_FIND_DATA file;
            HANDLE file_handle = FindFirstFile(path.c_str(), &file);
            if(file_handle == INVALID_HANDLE_VALUE) {
                return false;
            }
            FindClose(file_handle);
            return (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        }

        bool create_directory(const std::string& path_in) {
            std::vector<std::string> path;
            String::split_string(path_in, '/', path);
            std::string current;
            int start_at = 0;
            if(path_in.at(1) == ':') {
                current += path_in.at(0);
                current += path_in.at(1);
                start_at = 1;
            }
            else if(path_in.at(0) != '/' && path_in.at(0) != '\\') {
                current += get_current_working_directory();
            }
            for(size_t i = size_t(start_at); i < path.size(); i++) {
                current += "/";
                current += path[i];
                if(path[i].at(0) == '.' &&
                    path[i].at(1) == '.' &&
                    path[i].length() == 2
                ) {
                    continue;
                }
                if(!is_directory(current)) {
                    if(!::CreateDirectory(current.c_str(), nullptr)) {
                        Logger::err("OS")
                            << "Could not create directory "
                            << current << std::endl;
                        return false;
                    }
                }
            }
            return true;
        }

        bool delete_directory(const std::string& path) {
            return ::RemoveDirectory(path.c_str()) != FALSE;
        }

        bool delete_file(const std::string& path) {
            return ::DeleteFile(path.c_str()) != FALSE;
        }

        bool get_directory_entries(
            const std::string& path, std::vector<std::string>& result
        ) {
            std::string dirname = path;
            if(dirname.at(dirname.size() - 1) != '/' &&
                dirname.at(dirname.size() - 1) != '\\'
            ) {
                dirname += '/';
            }

            std::string current_directory = get_current_working_directory();

            bool dir_found = set_current_working_directory(dirname);
            if(!dir_found) {
                return false;
            }

            WIN32_FIND_DATA file;
            HANDLE file_handle = FindFirstFile("*.*", &file);
            if(file_handle != INVALID_HANDLE_VALUE) {
                do {
                    std::string file_name = file.cFileName;
                    if(file_name != "." && file_name != "..") {
                        file_name = dirname + file_name;
                        flip_slashes(file_name);
                        result.push_back(file_name);
                    }
                } while(FindNextFile(file_handle, &file));
                FindClose(file_handle);
            }
            set_current_working_directory(current_directory);
            return true;
        }

        std::string get_current_working_directory() {
            char buf[2048];
            std::string result = "";
            if(GetCurrentDirectory(sizeof(buf), buf)) {
                result = buf;
                flip_slashes(result);
            }
            return result;
        }

        bool set_current_working_directory(const std::string& path_in) {
            std::string path = path_in;
            if(
		path.at(path.size() - 1) != '/' &&
		path.at(path.size() - 1) != '\\') {
                path += "/";
            }
            return SetCurrentDirectory(path.c_str()) != -1;
        }

        bool rename_file(
            const std::string& old_name, const std::string& new_name
        ) {
            return ::rename(old_name.c_str(), new_name.c_str()) != -1;
        }

        Numeric::uint64 get_time_stamp(
            const std::string& path
        ) {
            WIN32_FILE_ATTRIBUTE_DATA infos;
            if(!GetFileAttributesEx(
		   path.c_str(), GetFileExInfoStandard, &infos)
	    ) {
                return 0;
            }
            return infos.ftLastWriteTime.dwLowDateTime;
        }

#else

        bool is_file(const std::string& path) {
            //   We use 'stat' and not 'lstat' since
            // we want to be able to follow symbolic
            // links (required for instance when testing
            // input path in GOMGEN, there can be some
            // symlinks in system includes)
            struct stat buff;
            if(stat(path.c_str(), &buff)) {
                return false;
            }
            return S_ISREG(buff.st_mode);
        }

        bool is_directory(const std::string& path) {
            //   We use 'stat' and not 'lstat' since
            // we want to be able to follow symbolic
            // links (required for instance when testing
            // input path in GOMGEN, there can be some
            // symlinks in system includes)
            struct stat buff;
            if(stat(path.c_str(), &buff)) {
                return false;
            }
            return S_ISDIR(buff.st_mode);
        }

        bool create_directory(const std::string& path_in) {
            std::vector<std::string> path;
            String::split_string(path_in, '/', path);
            std::string current;
            for(size_t i = 0; i < path.size(); i++) {
                current += "/";
                current += path[i];
                if(!is_directory(current)) {
                    if(mkdir(current.c_str(), 0755) != 0) {
                        Logger::err("OS")
                            << "Could not create directory "
                            << current << std::endl;
                        return false;
                    }
                }
            }
            return true;
        }

        bool delete_directory(const std::string& path) {
            return rmdir(path.c_str()) == 0;
        }

        bool delete_file(const std::string& path) {
            return unlink(path.c_str()) == 0;
        }

        bool get_directory_entries(
            const std::string& path, std::vector<std::string>& result
        ) {
            std::string dirname = path;
            if(dirname[dirname.length() - 1] != '/') {
                dirname += "/";
            }
            DIR* dir = opendir(dirname.c_str());
            if(dir == nullptr) {
                Logger::err("OS")
                    << "Could not open directory " << dirname
                    << std::endl;
                return false;
            }
            struct dirent* entry = readdir(dir);
            while(entry != nullptr) {
                std::string current = std::string(entry->d_name);
                // Ignore . and ..
                if(current != "." && current != "..") {
                    if(dirname != "./") {
                        current = dirname + current;
                    }
                    // Ignore symbolic links and other special Unix stuff
                    if(is_file(current) || is_directory(current)) {
                        result.push_back(current);
                    }
                }
                entry = readdir(dir);
            }
            closedir(dir);
            return true;
        }

        std::string get_current_working_directory() {
            char buff[4096];
            return std::string(getcwd(buff, 4096));
        }

        bool set_current_working_directory(const std::string& path) {
            return chdir(path.c_str()) == 0;
        }

        bool rename_file(
            const std::string& old_name, const std::string& new_name
        ) {
            if(is_file(new_name)) {
                return false;
            }
            return ::rename(old_name.c_str(), new_name.c_str()) == 0;
        }

        Numeric::uint64 get_time_stamp(
            const std::string& path
        ) {
            struct stat buffer;
            if(!stat(path.c_str(), &buffer)) {
                return Numeric::uint64(buffer.st_mtime);
            }
            return 0;
        }

#endif

        // OS-independent functions

        std::string extension(const std::string& path) {
            size_t len = path.length();
            if(len != 0) {
                for(size_t i = len - 1; i != 0; i--) {
                    if(path[i] == '/' || path[i] == '\\') {
                        break;
                    }
                    if(path[i] == '.') {
                        return String::to_lowercase(path.substr(i + 1));
                    }
                }
            }
            return std::string();
        }

        std::string base_name(const std::string& path, bool remove_extension) {
            long int len = (long int)(path.length());
            if(len == 0) {
                return std::string();
            }
            long int dot_pos = len;
            long int i;
            for(i = len - 1; i >= 0; i--) {
                if(path[size_t(i)] == '/' || path[size_t(i)] == '\\') {
                    break;
                }
                if(remove_extension && path[size_t(i)] == '.') {
                    dot_pos = i;
                }
            }
            return path.substr(size_t(i + 1), size_t(dot_pos - i - 1));
        }

        std::string dir_name(const std::string& path) {
            size_t len = path.length();
            if(len != 0) {
                for(size_t i = len - 1; i != 0; i--) {
                    if(path[i] == '/' || path[i] == '\\') {
                        return path.substr(0, i);
                    }
                }
            }
            return ".";
        }

        void get_directory_entries(
            const std::string& path,
            std::vector<std::string>& result, bool recursive
        ) {
	    // TODO: seems to be bugged, enters infinite recursion...
            get_directory_entries(path, result);
            if(recursive) {
                for(size_t i = 0; i < result.size(); i++) {
                    if(is_directory(result[i])) {
                        get_directory_entries(result[i], result, true);
                    }
                }
            }
        }

        void get_files(
            const std::string& path,
            std::vector<std::string>& result, bool recursive
        ) {
            std::vector<std::string> entries;
            get_directory_entries(path, entries, recursive);
            for(size_t i = 0; i < entries.size(); i++) {
                if(is_file(entries[i])) {
                    result.push_back(entries[i]);
                }
            }
        }

        void get_subdirectories(
            const std::string& path,
            std::vector<std::string>& result, bool recursive
        ) {
            std::vector<std::string> entries;
            get_directory_entries(path, entries, recursive);
            for(size_t i = 0; i < entries.size(); i++) {
                if(is_directory(entries[i])) {
                    result.push_back(entries[i]);
                }
            }
        }

        void flip_slashes(std::string& s) {
            for(size_t i = 0; i < s.length(); i++) {
                if(s[i] == '\\') {
                    s[i] = '/';
                }
            }
        }

        bool copy_file(const std::string& from, const std::string& to) {
            FILE* fromf = fopen(from.c_str(), "rb");
            if(fromf == nullptr) {
                Logger::err("FileSyst")
		    << "Could not open source file:" << from << std::endl;
                return false;
            }
            FILE* tof = fopen(to.c_str(),"wb");
            if(tof == nullptr) {
                Logger::err("FileSyst")
		    << "Could not create file:" << to << std::endl;
                fclose(fromf);
                return false;
            }

            bool result = true;
            const size_t buff_size = 4096;
            char buff[buff_size];
            size_t rdsize = 0;
            do {
                rdsize = fread(buff, 1, buff_size, fromf);
                if(fwrite(buff, 1, rdsize, tof) != rdsize) {
                    Logger::err("FileSyst") << "I/O error when writing to file:"
                                            << to << std::endl;
                    result = false;
                    break;
                }
            } while(rdsize == 4096);
            
            fclose(fromf);
            fclose(tof);
            return result;
        }

        void set_executable_flag(const std::string& filename) {
            geo_argused(filename);
#ifdef GEO_OS_UNIX
            if(::chmod(filename.c_str(), 0755) != 0) {
                Logger::err("FileSyst")
                    << "Could not change file permissions for:"
                    << filename << std::endl;
            }
#endif            
        }

        void GEOGRAM_API touch(const std::string& filename) {
#ifdef GEO_OS_APPLE
           {
                struct stat buff;
                int rc = stat(filename.c_str(), &buff); 
                if(rc != 0) {  // FABIEN NOT SURE WE GET THE TOUCH
                    Logger::err("FileSystem")
                        << "Could not touch file:"
                        << filename
                        << std::endl;
                }
            }
#elif defined(GEO_OS_UNIX)
            {
                int rc = utimensat(
                    AT_FDCWD,
                    filename.c_str(),
                    nullptr,
                    0
                );
                if(rc != 0) {
                    Logger::err("FileSystem")
                        << "Could not touch file:"
                        << filename
                        << std::endl;
                }
            }
#elif defined GEO_OS_WINDOWS
            {
                HANDLE hfile = CreateFile(
                    filename.c_str(),
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                if(hfile == INVALID_HANDLE_VALUE) {
                    Logger::err("FileSystem")
                        << "Could not touch file:"
                        << filename
                        << std::endl;
                }
                SYSTEMTIME now_system;
                FILETIME now_file;
                GetSystemTime(&now_system);
                SystemTimeToFileTime(&now_system, &now_file);
                SetFileTime(hfile,nullptr,&now_file,&now_file);
                CloseHandle(hfile);
            }
#endif            
        }
        
        std::string normalized_path(const std::string& path_in) {

            if(path_in == "") {
                return "";
            }
            
            std::string path = path_in;
            std::string result;

#ifdef GEO_OS_UNIX
            // If this is a relative path, prepend "./"
            if(path[0] != '/') {
                path = "./" + path;
            }

            char buffer[PATH_MAX];
            char* p = realpath(path.c_str(), buffer);
            if(p != nullptr) {
                result = std::string(p);
            } else {
                // realpath() only works for existing paths and existing file,
                // therefore we attempt calling it on the input path by adding
                // one component at a time.
                size_t pos = 1;
                while(pos != std::string::npos) {
                    pos = path.find('/',pos);
                    if(pos != std::string::npos) {
                        std::string path_part = path.substr(0,pos);
                        p = realpath(path_part.c_str(), buffer);
                        if(p == nullptr) {
                            break;
                        } else {
                            result = std::string(p) +
                                path.substr(pos, path.length()-pos);
                        }
                        ++pos;
                        if(pos == path.length()) {
                            break;
                        }
                    } 
                }
            }
#endif
            
#ifdef GEO_OS_WINDOWS
            TCHAR buffer[MAX_PATH];
            GetFullPathName(path.c_str(), MAX_PATH, buffer, nullptr);
            result = std::string(buffer);
#endif
            
            flip_slashes(result);
            return result;
        }


        std::string home_directory() {
            std::string home;
#if defined GEO_OS_WINDOWS
            wchar_t folder[MAX_PATH+1];
            HRESULT hr = SHGetFolderPathW(0, CSIDL_PROFILE, 0, 0, folder);
            if (SUCCEEDED(hr)) {
                char result[MAX_PATH+1];
                wcstombs(result, folder, MAX_PATH);
                home=std::string(result);
                flip_slashes(home);
            }
#elif defined GEO_OS_EMSCRIPTEN
            home="/";
#else            
            char* result = getenv("HOME");
            if(result != nullptr) {
                home=result;
            }
#endif
            return home;
        }

        std::string documents_directory() {
            std::string home;
#if defined GEO_OS_WINDOWS
            wchar_t folder[MAX_PATH+1];
            HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, folder);
            if (SUCCEEDED(hr)) {
                char result[MAX_PATH+1];
                wcstombs(result, folder, MAX_PATH);
                home=std::string(result);
                flip_slashes(home);
            }
#elif defined GEO_OS_EMSCRIPTEN
            home="/";
#elif defined GEO_OS_ANDROID
            char* result = getenv("EXTERNAL_STORAGE");
            if(result != nullptr) {
                home=result;
            }
#else            
            char* result = getenv("HOME");
            if(result != nullptr) {
                home=result;
            }
#endif
            return home;
        }
        
    }

    
}


/******* extracted from ../basic/packed_arrays.cpp *******/


namespace {

    using namespace GEO;

    std::string percent_str(index_t num, index_t denom) {
        if(denom == 0) {
            return String::to_string(num);
        }
        double x = double(num) / double(denom) * 100.0;
        return String::to_string(num) + "(" + String::to_string(x) + "%)";
    }
}

namespace GEO {

    PackedArrays::PackedArrays() {
        nb_arrays_ = 0;
        Z1_block_size_ = 0;
        Z1_stride_ = 0;
        Z1_ = nullptr;
        ZV_ = nullptr;
        thread_safe_ = false;
    }

    void PackedArrays::show_stats() {
        index_t nb_items_in_Z1 = 0;
        index_t nb_items_in_ZV = 0;
        index_t nb_arrays_in_ZV = 0;
        index_t nb_items = 0;
        for(index_t i = 0; i < nb_arrays_; i++) {
            index_t sz = array_size(i);
            nb_items += sz;
            if(sz > Z1_block_size_) {
                nb_items_in_ZV += (sz - Z1_block_size_);
                nb_arrays_in_ZV++;
            }
            nb_items_in_Z1 += std::min(sz, Z1_block_size_);
        }

        Logger::out("PArrays")
            << "stats (nb_arrays=" << nb_arrays_
            << ", Z1 block size=" << Z1_block_size_ << ") "
            << (static_mode() ? "static" : "dynamic")
            << std::endl;

        index_t Z1_total = nb_arrays_ * Z1_block_size_;

        Logger::out("PArrays")
            << "Z1 filling:"
            << percent_str(nb_items_in_Z1, Z1_total) << std::endl;

        if(!static_mode()) {
            Logger::out("PArrays")
                << "arrays in ZV:" << percent_str(nb_arrays_in_ZV, nb_arrays_)
                << std::endl;
            Logger::out("PArrays")
                << "items  in Z1:" << percent_str(nb_items_in_Z1, nb_items)
                << std::endl;
            Logger::out("PArrays")
                << "items  in ZV:" << percent_str(nb_items_in_ZV, nb_items)
                << std::endl;
        }
    }

    PackedArrays::~PackedArrays() {
        clear();
    }

    void PackedArrays::clear() {
        if(ZV_ != nullptr) {
            for(index_t i = 0; i < nb_arrays_; i++) {
                free(ZV_[i]);
            }
            free(ZV_);
            ZV_ = nullptr;
        }
        nb_arrays_ = 0;
        Z1_block_size_ = 0;
        Z1_stride_ = 0;
        free(Z1_);
        Z1_ = nullptr;
    }

    void PackedArrays::set_thread_safe(bool x) {
        thread_safe_ = x;
        if(x) {
            Z1_spinlocks_.resize(nb_arrays_);
        } else {
            Z1_spinlocks_.clear();
        }
    }

    void PackedArrays::init(
        index_t nb_arrays,
        index_t Z1_block_size,
        bool static_mode
    ) {
        clear();
        nb_arrays_ = nb_arrays;
        Z1_block_size_ = Z1_block_size;
        Z1_stride_ = Z1_block_size_ + 1;  // +1 for storing array size.
        Z1_ = (index_t*) calloc(
            nb_arrays_, sizeof(index_t) * Z1_stride_
        );
        if(!static_mode) {
            ZV_ = (index_t**) calloc(
                nb_arrays_, sizeof(index_t*)
            );
        }
        if(thread_safe_) {
            Z1_spinlocks_.resize(nb_arrays_);
        }
    }

    void PackedArrays::get_array(
        index_t array_index, index_t* array, bool lock
    ) const {
        geo_debug_assert(array_index < nb_arrays_);
        if(lock) {
            lock_array(array_index);
        }
        const index_t* array_base = Z1_ + array_index * Z1_stride_;
        index_t array_size = *array_base;
        index_t nb = array_size;
        array_base++;
        index_t nb_in_block = std::min(nb, Z1_block_size_);
        Memory::copy(array, array_base, sizeof(index_t) * nb_in_block);
        if(nb > nb_in_block) {
            nb -= nb_in_block;
            array += nb_in_block;
            array_base = ZV_[array_index];
            Memory::copy(array, array_base, sizeof(index_t) * nb);
        }
        if(lock) {
            unlock_array(array_index);
        }
    }

    void PackedArrays::set_array(
        index_t array_index,
        index_t array_size, const index_t* array,
        bool lock
    ) {
        geo_debug_assert(array_index < nb_arrays_);
        if(lock) {
            lock_array(array_index);
        }
        index_t* array_base = Z1_ + array_index * Z1_stride_;
        index_t old_array_size = *array_base;
        array_base++;
        if(array_size != old_array_size) {
            resize_array(array_index, array_size, false);
        }
        index_t nb = array_size;
        index_t nb_in_block = std::min(nb, Z1_block_size_);
        Memory::copy(array_base, array, sizeof(index_t) * nb_in_block);
        if(nb > nb_in_block) {
            nb -= nb_in_block;
            array += nb_in_block;
            array_base = ZV_[array_index];
            Memory::copy(array_base, array, sizeof(index_t) * nb);
        }
        if(lock) {
            unlock_array(array_index);
        }
    }

    void PackedArrays::resize_array(
        index_t array_index, index_t array_size, bool lock
    ) {
        geo_debug_assert(array_index < nb_arrays_);
        if(lock) {
            lock_array(array_index);
        }
        index_t* array_base = Z1_ + array_index * Z1_stride_;
        index_t old_array_size = *array_base;
        if(old_array_size != array_size) {
            *array_base = array_size;
            if(static_mode()) {
                geo_assert(array_size <= Z1_block_size_);
            } else {
                index_t nb_in_ZV =
                    (array_size > Z1_block_size_) ?
                    array_size - Z1_block_size_ : 0;
                ZV_[array_index] = (index_t*) realloc(
                    ZV_[array_index], sizeof(index_t) * nb_in_ZV
                );
            }
        }
        if(lock) {
            unlock_array(array_index);
        }
    }
}


/******* extracted from ../basic/progress.cpp *******/

#include <stack>

namespace {

    using namespace GEO;

    ProgressClient_var progress_client_;
    std::stack<const ProgressTask*> progress_tasks_;
    bool task_canceled_ = false;

    void begin_task(const ProgressTask* task) {
        task_canceled_ = false;
        progress_tasks_.push(task);

        if(progress_client_) {
            progress_client_->begin();
        }
    }

    void reset_task(const ProgressTask* task) {
        geo_argused(task);
        task_canceled_ = false;
    }

    void task_progress(index_t step, index_t percent) {
        if(task_canceled_) {
            throw TaskCanceled();
        }

        if(progress_client_) {
            progress_client_->progress(step, percent);
        }
    }

    void end_task(const ProgressTask* task) {
        geo_assert(!progress_tasks_.empty());
        geo_assert(progress_tasks_.top() == task);

        if(progress_client_) {
            progress_client_->end(task_canceled_);
        }

        progress_tasks_.pop();
        if(progress_tasks_.empty()) {
            task_canceled_ = false;
        }
    }

    class TerminalProgressClient : public ProgressClient {
    public:
        
	void begin() override {
            const ProgressTask* task = Progress::current_task();
            CmdLine::ui_progress(task->task_name(), 0, 0);
        }

        
	void progress(index_t step, index_t percent) override {
            const ProgressTask* task = Progress::current_task();
            CmdLine::ui_progress(task->task_name(), step, percent);
        }

        
	void end(bool canceled) override {
            const ProgressTask* task = Progress::current_task();
            double elapsed = SystemStopwatch::now() - task->start_time();
            if(canceled) {
                CmdLine::ui_progress_canceled(
                    task->task_name(), elapsed, task->percent()
                );
            } else {
                CmdLine::ui_progress_time(task->task_name(), elapsed);
            }
        }

    protected:
        
	~TerminalProgressClient() override {
        }
    };
}



namespace GEO {

    const char* TaskCanceled::what() const GEO_NOEXCEPT {
        return "Task canceled";
    }

    

    namespace Progress {

        void initialize() {
            set_client(new TerminalProgressClient());
        }

        void terminate() {
            set_client(nullptr);
        }

        void set_client(ProgressClient* client) {
            progress_client_ = client;
        }

        const ProgressTask* current_task() {
            return progress_tasks_.empty() ? nullptr : progress_tasks_.top();
        }

        void cancel() {
            if(!progress_tasks_.empty()) {
                task_canceled_ = true;
            }
        }

        bool is_canceled() {
            return task_canceled_;
        }

        void clear_canceled() {
            task_canceled_ = false;
        }
    }

    

    ProgressClient::~ProgressClient() {
    }

    

    ProgressTask::ProgressTask(
        const std::string& task_name, index_t max_steps, bool quiet
    ) :
        task_name_(task_name),
        start_time_(SystemStopwatch::now()),
        quiet_(quiet),
        max_steps_(std::max(index_t(1), max_steps)),
        step_(0),
        percent_(0)
    {
        if(!quiet_) {
            begin_task(this);
        }
    }

    ProgressTask::ProgressTask(
        const std::string& task_name, index_t max_steps
    ) :
        task_name_(task_name),
        start_time_(SystemStopwatch::now()),
        quiet_(Logger::instance()->is_quiet()),
        max_steps_(std::max(index_t(1), max_steps)),
        step_(0),
        percent_(0)
    {
        if(!quiet_) {
            begin_task(this);
        }
    }

    
    ProgressTask::~ProgressTask() {
        if(!quiet_) {
            end_task(this);
        }
    }

    void ProgressTask::reset() {
        start_time_ = SystemStopwatch::now();
        reset_task(this);
        progress(0);
    }

    void ProgressTask::reset(index_t max_steps) {
        max_steps_ = std::max(index_t(1), max_steps);
        reset();
    }

    void ProgressTask::next() {
        step_++;
	step_ = std::min(step_, max_steps_);
        update();
    }

    void ProgressTask::progress(index_t step) {
        if(step_ != step) {
            step_ = step;
            update();
        }
    }

    bool ProgressTask::is_canceled() const {
        return task_canceled_;
    }

    void ProgressTask::update() {
        percent_ = std::min(index_t(100), index_t(step_ * 100 / max_steps_));
        if(!quiet_) {
            task_progress(step_, percent_);
        }
    }
}


/******* extracted from ../basic/process.cpp *******/


#ifdef GEO_OPENMP
#include <omp.h>
#endif

namespace {
    using namespace GEO;

    ThreadManager_var thread_manager_;
    int running_threads_invocations_ = 0;

    bool multithreading_initialized_ = false;
    bool multithreading_enabled_ = true;

    index_t max_threads_initialized_ = false;
    index_t max_threads_ = 0;

    bool fpe_initialized_ = false;
    bool fpe_enabled_ = false;

    bool cancel_initialized_ = false;
    bool cancel_enabled_ = false;

    double start_time_ = 0.0;

    

    class ProcessEnvironment : public Environment {
    protected:
        virtual bool get_local_value(
            const std::string& name, std::string& value
        ) const {
            if(name == "sys:nb_cores") {
                value = String::to_string(Process::number_of_cores());
                return true;
            }
            if(name == "sys:multithread") {
                value = String::to_string(multithreading_enabled_);
                return true;
            }
            if(name == "sys:max_threads") {
                value = String::to_string(
                    Process::maximum_concurrent_threads()
                );
                return true;
            }
            if(name == "sys:FPE") {
                value = String::to_string(fpe_enabled_);
                return true;
            }
            if(name == "sys:cancel") {
                value = String::to_string(cancel_enabled_);
                return true;
            }
            if(name == "sys:assert") {
                value = assert_mode() == ASSERT_THROW ? "throw" : "abort";
                return true;
            }
            return false;
        }

        virtual bool set_local_value(
            const std::string& name, const std::string& value
        ) {
            if(name == "sys:multithread") {
                Process::enable_multithreading(String::to_bool(value));
                return true;
            }
            if(name == "sys:max_threads") {
                Process::set_max_threads(String::to_uint(value));
                return true;
            }
            if(name == "sys:FPE") {
                Process::enable_FPE(String::to_bool(value));
                return true;
            }
            if(name == "sys:cancel") {
                Process::enable_cancel(String::to_bool(value));
                return true;
            }
            if(name == "sys:assert") {
                if(value == "throw") {
                    set_assert_mode(ASSERT_THROW);
                    return true;
                }
		if(value == "abort") {
                    set_assert_mode(ASSERT_ABORT);
                    return true;
                }
		if(value == "breakpoint") {
                    set_assert_mode(ASSERT_BREAKPOINT);
                    return true;
		}
                Logger::err("Process")
                    << "Invalid value for property sys:abort: "
                    << value
                    << std::endl;
                return false;
            }
            return false;
        }

        
        virtual ~ProcessEnvironment() {
        }
    };

    

#ifdef GEO_OPENMP

    class GEOGRAM_API OMPThreadManager : public ThreadManager {
    public:
        OMPThreadManager() {
            omp_init_lock(&lock_);
        }

        
        virtual index_t maximum_concurrent_threads() {
            return Process::number_of_cores();
        }

        
        virtual void enter_critical_section() {
            omp_set_lock(&lock_);
        }

        
        virtual void leave_critical_section() {
            omp_unset_lock(&lock_);
        }

    protected:
        
        virtual ~OMPThreadManager() {
            omp_destroy_lock(&lock_);
        }

        
        virtual void run_concurrent_threads(
            ThreadGroup& threads, index_t max_threads
        ) {
            // TODO: take max_threads_ into account
            geo_argused(max_threads);

#pragma omp parallel for schedule(dynamic)
            for(index_t i = 0; i < threads.size(); i++) {
                set_thread_id(threads[i],i);
                set_current_thread(threads[i]);
                threads[i]->run();
            }
        }

    private:
        omp_lock_t lock_;
    };

#endif
}


namespace {
    thread_local Thread* geo_current_thread_ = nullptr;
}

namespace GEO {

    void Thread::set_current(Thread* thread) {
        geo_current_thread_ = thread;
    }

    Thread* Thread::current() {
        return geo_current_thread_;
    }
    
    Thread::~Thread() {
    }

    

    ThreadManager::~ThreadManager() {
    }

    void ThreadManager::run_threads(ThreadGroup& threads) {
        index_t max_threads = maximum_concurrent_threads();
        if(Process::multithreading_enabled() && max_threads > 1) {
            run_concurrent_threads(threads, max_threads);
        } else {
            for(index_t i = 0; i < threads.size(); i++) {
                threads[i]->run();
            }
        }
    }

    

    MonoThreadingThreadManager::~MonoThreadingThreadManager() {
    }

    void MonoThreadingThreadManager::run_concurrent_threads(
        ThreadGroup& threads, index_t max_threads
    ) {
        geo_argused(threads);
        geo_argused(max_threads);
        geo_assert_not_reached;
    }

    index_t MonoThreadingThreadManager::maximum_concurrent_threads() {
        return 1;
    }

    void MonoThreadingThreadManager::enter_critical_section() {
    }

    void MonoThreadingThreadManager::leave_critical_section() {
    }

    

    namespace Process {

        // OS dependent functions implemented in process_unix.cpp and
        // process_win.cpp

        bool os_init_threads();
        void os_brute_force_kill();
        bool os_enable_FPE(bool flag);
        bool os_enable_cancel(bool flag);
        void os_install_signal_handlers();
        index_t os_number_of_cores();
        size_t os_used_memory();
        size_t os_max_used_memory();
        std::string os_executable_filename();
        
        void initialize(int flags) {

            Environment* env = Environment::instance();
            env->add_environment(new ProcessEnvironment);

            if(!os_init_threads()) {
#ifdef GEO_OPENMP
                Logger::out("Process")
                    << "Using OpenMP threads"
                    << std::endl;
                set_thread_manager(new OMPThreadManager);
#else
                Logger::out("Process")
                    << "Multithreading not supported, going monothread"
                    << std::endl;
                set_thread_manager(new MonoThreadingThreadManager);
#endif
            }

	    if(
		(::getenv("GEO_NO_SIGNAL_HANDLER") == nullptr) &&
		(flags & GEOGRAM_INSTALL_HANDLERS) != 0
	    ) {
		os_install_signal_handlers();
	    }
	    
            // Initialize Process default values
            enable_multithreading(multithreading_enabled_);
            set_max_threads(number_of_cores());
            enable_FPE(fpe_enabled_);
            enable_cancel(cancel_enabled_);

            start_time_ = SystemStopwatch::now();
        }

        void show_stats() {

            Logger::out("Process") << "Total elapsed time: " 
                                   << SystemStopwatch::now() - start_time_
                                   << "s" << std::endl;

            const size_t K=size_t(1024);
            const size_t M=K*K;
            const size_t G=K*M;
            
            size_t max_mem = Process::max_used_memory() ;
            size_t r = max_mem;
            
            size_t mem_G = r / G;
            r = r % G;
            size_t mem_M = r / M;
            r = r % M;
            size_t mem_K = r / K;
            r = r % K;
            
            std::string s;
            if(mem_G != 0) {
                s += String::to_string(mem_G)+"G ";
            }
            if(mem_M != 0) {
                s += String::to_string(mem_M)+"M ";
            }
            if(mem_K != 0) {
                s += String::to_string(mem_K)+"K ";
            }
            if(r != 0) {
                s += String::to_string(r);
            }

            Logger::out("Process") << "Maximum used memory: " 
                                   << max_mem << " (" << s << ")"
                                   << std::endl;
        }

        void terminate() {
            thread_manager_.reset();
        }

        void brute_force_kill() {
            os_brute_force_kill();
        }

        index_t number_of_cores() {
            static index_t result = 0;
            if(result == 0) {
#ifdef GEO_NO_THREAD_LOCAL
		// Deactivate multithreading if thread_local is
		// not supported (e.g. with old OS-X).
		result = 1;
#else		
                result = os_number_of_cores();
#endif		
            }
            return result;
        }

        size_t used_memory() {
            return os_used_memory();
        }

        size_t max_used_memory() {
            return os_max_used_memory();
        }

        std::string executable_filename() {
            return os_executable_filename();
        }
        
        void set_thread_manager(ThreadManager* thread_manager) {
            thread_manager_ = thread_manager;
        }

        void run_threads(ThreadGroup& threads) {
            running_threads_invocations_++;
            thread_manager_->run_threads(threads);
            running_threads_invocations_--;
        }

        void enter_critical_section() {
            thread_manager_->enter_critical_section();
        }

        void leave_critical_section() {
            thread_manager_->leave_critical_section();
        }

        bool is_running_threads() {
#ifdef GEO_OPENMP
            return (
		omp_in_parallel() ||
		(running_threads_invocations_ > 0)
	    );	    
#else	    
            return running_threads_invocations_ > 0;
#endif	    
        }

        bool multithreading_enabled() {
            return multithreading_enabled_;
        }

        void enable_multithreading(bool flag) {
            if(
                multithreading_initialized_ &&
                multithreading_enabled_ == flag
            ) {
                return;
            }
            multithreading_initialized_ = true;
            multithreading_enabled_ = flag;
            if(multithreading_enabled_) {
                Logger::out("Process")
                    << "Multithreading enabled" << std::endl
                    << "Available cores = " << number_of_cores()
                    << std::endl;
                // Logger::out("Process")
                //    << "Max. concurrent threads = "
                //    << maximum_concurrent_threads() << std::endl ;
                if(number_of_cores() == 1) {
                    Logger::warn("Process")
                        << "Processor is not a multicore"
			<< "(or multithread is not supported)"
                        << std::endl;
                }
                if(thread_manager_ == nullptr) {
                    Logger::warn("Process")
                        << "Missing multithreading manager"
                        << std::endl;
                }
            } else {
                Logger::out("Process")
                    << "Multithreading disabled" << std::endl;
            }
        }

        index_t max_threads() {
            return max_threads_initialized_
                   ? max_threads_
                   : number_of_cores();
        }

        void set_max_threads(index_t num_threads) {
            if(
                max_threads_initialized_ &&
                max_threads_ == num_threads
            ) {
                return;
            }
            max_threads_initialized_ = true;
            if(num_threads == 0) {
                num_threads = 1;
            } else if(num_threads > number_of_cores()) {
                Logger::warn("Process")
                    << "Cannot allocate " << num_threads 
                    << " for multithreading"
                    << std::endl;
                num_threads = number_of_cores();
            }
            max_threads_ = num_threads;
            Logger::out("Process")
                << "Max used threads = " << max_threads_
                << std::endl;
        }

        index_t maximum_concurrent_threads() {
            if(!multithreading_enabled_ || thread_manager_ == nullptr) {
                return 1;
            }
            return max_threads_;
            /*
               // commented out for now, since under Windows,
               // it seems that maximum_concurrent_threads() does not
               // report the number of hyperthreaded cores.
                        return
                            geo_min(
                                thread_manager_->maximum_concurrent_threads(),
                                max_threads_
                            ) ;
             */
        }

        bool FPE_enabled() {
            return fpe_enabled_;
        }

        void enable_FPE(bool flag) {
            if(fpe_initialized_ && fpe_enabled_ == flag) {
                return;
            }
            fpe_initialized_ = true;
            fpe_enabled_ = flag;
	    os_enable_FPE(flag);
        }

        bool cancel_enabled() {
            return cancel_enabled_;
        }

        void enable_cancel(bool flag) {
            if(cancel_initialized_ && cancel_enabled_ == flag) {
                return;
            }
            cancel_initialized_ = true;
            cancel_enabled_ = flag;

            if(os_enable_cancel(flag)) {
                Logger::out("Process")
                    << (flag ? "Cancel mode enabled" : "Cancel mode disabled")
                    << std::endl;
            } else {
                Logger::warn("Process")
                    << "Cancel mode not implemented" << std::endl;
            }
        }
    }
}


namespace {
    using namespace GEO;

    class ParallelThread : public Thread {
    public:
	ParallelThread(
	    std::function<void(void)> func
	) : func_(func) {
	}

        void run() override {
	    func_();
        }
    private:
	std::function<void()> func_;
    };


    class ParallelForThread : public Thread {
    public:

	ParallelForThread(
	    std::function<void(index_t)> func,
	    index_t from, index_t to, index_t step=1
	) : func_(func), from_(from), to_(to), step_(step) {
	}

        void run() override {
            for(index_t i = from_; i < to_; i += step_) {
                func_(i);
            }
        }
    private:
	std::function<void(index_t)> func_;
	index_t from_;
	index_t to_;
	index_t step_;
    };

    class ParallelForSliceThread : public Thread {
    public:

	ParallelForSliceThread(
	    std::function<void(index_t,index_t)> func,
	    index_t from, index_t to
	) : func_(func), from_(from), to_(to) {
	}

        void run() override {
	    func_(from_, to_);
        }
    private:
	std::function<void(index_t,index_t)> func_;
	index_t from_;
	index_t to_;
    };
    
}

namespace GEO {

    void parallel_for(
        index_t from, index_t to, std::function<void(index_t)> func,
        index_t threads_per_core, bool interleaved 
    ) {
#ifdef GEO_OS_WINDOWS
        // TODO: This is a limitation of WindowsThreadManager, to be fixed.
        threads_per_core = 1;
#endif

        index_t nb_threads = std::min(
            to - from,
            Process::maximum_concurrent_threads() * threads_per_core
        );

	nb_threads = std::max(index_t(1), nb_threads);
	
        index_t batch_size = (to - from) / nb_threads;
        if(Process::is_running_threads() || nb_threads == 1) {
            for(index_t i = from; i < to; i++) {
                func(i);
            }
        } else {
            ThreadGroup threads;
            if(interleaved) {
                for(index_t i = 0; i < nb_threads; i++) {
                    threads.push_back(
                        new ParallelForThread(
                            func, from + i, to, nb_threads
                        )
                    );
                }
            } else {
                index_t cur = from;
                for(index_t i = 0; i < nb_threads; i++) {
                    if(i == nb_threads - 1) {
                        threads.push_back(
                            new ParallelForThread(
                                func, cur, to
                            )
                        );
                    } else {
                        threads.push_back(
                            new ParallelForThread(
                                func, cur, cur + batch_size
                            )
                        );
                    }
                    cur += batch_size;
                }
            }
            Process::run_threads(threads);
        }
    }


    void parallel_for_slice(
	index_t from, index_t to, std::function<void(index_t, index_t)> func,
        index_t threads_per_core 
    ) {
#ifdef GEO_OS_WINDOWS
        // TODO: This is a limitation of WindowsThreadManager, to be fixed.
        threads_per_core = 1;
#endif

        index_t nb_threads = std::min(
            to - from,
            Process::maximum_concurrent_threads() * threads_per_core
        );

	nb_threads = std::max(index_t(1), nb_threads);
	
        index_t batch_size = (to - from) / nb_threads;
        if(Process::is_running_threads() || nb_threads == 1) {
	    func(from, to);
        } else {
            ThreadGroup threads;
	    index_t cur = from;
	    for(index_t i = 0; i < nb_threads; i++) {
		if(i == nb_threads - 1) {
		    threads.push_back(
			new ParallelForSliceThread(
			    func, cur, to
			  )
			);
		} else {
		    threads.push_back(
			new ParallelForSliceThread(
			    func, cur, cur + batch_size
                           )
                        );
		}
		cur += batch_size;
	    }
            Process::run_threads(threads);
        }
    }

    void parallel(
	std::function<void()> f1,
	std::function<void()> f2
    ) {
        if(Process::is_running_threads()) {
	    f1();
	    f2();
        } else {
            ThreadGroup threads;
	    threads.push_back(new ParallelThread(f1));
	    threads.push_back(new ParallelThread(f2));
            Process::run_threads(threads);
        }
    }
    

    void parallel(
	std::function<void()> f1,
	std::function<void()> f2,
	std::function<void()> f3,
	std::function<void()> f4
    ) {
        if(Process::is_running_threads()) {
	    f1();
	    f2();
	    f3();
	    f4();
        } else {
            ThreadGroup threads;
	    threads.push_back(new ParallelThread(f1));
	    threads.push_back(new ParallelThread(f2));
	    threads.push_back(new ParallelThread(f3));
	    threads.push_back(new ParallelThread(f4));
            Process::run_threads(threads);
        }
    }

    
    void parallel(
	std::function<void()> f1,
	std::function<void()> f2,
	std::function<void()> f3,
	std::function<void()> f4,
	std::function<void()> f5,
	std::function<void()> f6,
	std::function<void()> f7,
	std::function<void()> f8	 
    ) {
        if(Process::is_running_threads()) {
	    f1();
	    f2();
	    f3();
	    f4();
	    f5();
	    f6();
	    f7();
	    f8();
        } else {
            ThreadGroup threads;
	    threads.push_back(new ParallelThread(f1));
	    threads.push_back(new ParallelThread(f2));
	    threads.push_back(new ParallelThread(f3));
	    threads.push_back(new ParallelThread(f4));
	    threads.push_back(new ParallelThread(f5));
	    threads.push_back(new ParallelThread(f6));
	    threads.push_back(new ParallelThread(f7));
	    threads.push_back(new ParallelThread(f8));	    
            Process::run_threads(threads);
        }
    }
}


/******* extracted from ../basic/process_unix.cpp *******/


#ifdef GEO_OS_UNIX


#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <fenv.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <new>

#ifdef GEO_OS_APPLE
#include <mach-o/dyld.h>
#include <xmmintrin.h>
#endif

#define GEO_USE_PTHREAD_MANAGER

// Suppresses a warning with CLANG when sigaction is used.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif

namespace {

    using namespace GEO;

#ifdef GEO_OS_ANDROID

    int android_get_number_of_cores() {
        FILE* fp;
        int res, i = -1, j = -1;
        /* open file */
        fp = fopen("/sys/devices/system/cpu/present", "r");
        if(fp == 0) {
            return -1; /* failure */
        }

        /* read and interpret line */
        res = fscanf(fp, "%d-%d", &i, &j);

        /* close file */
        fclose(fp);

        /* interpret result */
        if(res == 1 && i == 0) {
            /* single-core */
            return 1;
        }

        if(res == 2 && i == 0) {
            /* 2+ cores */
            return j + 1;
        }

        return -1; /* failure */
    }

#endif

#ifdef GEO_USE_PTHREAD_MANAGER

    class GEOGRAM_API PThreadManager : public ThreadManager {
    public:
        PThreadManager() {
            // For now, I do not trust pthread_mutex_xxx functions
            // under Android, so I'm using assembly functions
            // from atomics (I'm sure they got the right memory
            // barriers for SMP).
#if defined(GEO_OS_ANDROID) || defined(GEO_OS_RASPBERRY)
            mutex_ = 0;
#else
            pthread_mutex_init(&mutex_, nullptr);
#endif
            pthread_attr_init(&attr_);
            pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_JOINABLE);
        }

        
        index_t maximum_concurrent_threads() override {
            return Process::number_of_cores();
        }

        
	void enter_critical_section() override {
#if defined(GEO_OS_RASPBERRY)
            lock_mutex_arm32(&mutex_);
#elif defined(GEO_OS_ANDROID)
            lock_mutex_android(&mutex_);
#else
            pthread_mutex_lock(&mutex_);
#endif
        }

        
	void leave_critical_section() override {
#if defined(GEO_OS_RASPBERRY)
            unlock_mutex_arm32(&mutex_);	    
#elif defined(GEO_OS_ANDROID)
            unlock_mutex_android(&mutex_);
#else
            pthread_mutex_unlock(&mutex_);
#endif
        }

    protected:
        
	~PThreadManager() override {
            pthread_attr_destroy(&attr_);
#ifndef GEO_OS_ANDROID
            pthread_mutex_destroy(&mutex_);
#endif
        }

        static void* run_thread(void* thread_in) {
            Thread* thread = reinterpret_cast<Thread*>(thread_in);
            // Sets the thread-local-storage instance pointer, so
            // that Thread::current() can retrieve it.
            set_current_thread(thread);
            thread->run();
            return nullptr;
        }

        
	void run_concurrent_threads (
            ThreadGroup& threads, index_t max_threads
        ) override {
            // TODO: take max_threads into account
            geo_argused(max_threads);

            thread_impl_.resize(threads.size());
            for(index_t i = 0; i < threads.size(); i++) {
                Thread* T = threads[i];
                set_thread_id(T,i);
                pthread_create(
                    &thread_impl_[i], &attr_, &run_thread, T
                );
            }
            for(index_t i = 0; i < threads.size(); ++i) {
                pthread_join(thread_impl_[i], nullptr);
            }

        }

    private:
#if defined(GEO_OS_RASPBERRY)
        arm32_mutex_t mutex_;	
#elif defined(GEO_OS_ANDROID)
        android_mutex_t mutex_;
#else
        pthread_mutex_t mutex_;
#endif
        pthread_attr_t attr_;
        std::vector<pthread_t> thread_impl_;
    };

#endif

    GEO_NORETURN_DECL void abnormal_program_termination(
        const char* message = nullptr
    ) GEO_NORETURN;
    
    void abnormal_program_termination(const char* message) {
        if(message != nullptr) {
            // Do not use Logger here!
            std::cout
                << "Abnormal program termination: "
                << message << std::endl;
        }
        exit(1);
    }

    GEO_NORETURN_DECL void signal_handler(int signal) GEO_NORETURN;
    
    void signal_handler(int signal) {
        const char* sigstr = strsignal(signal);
        std::ostringstream os;
        os << "received signal " << signal << " (" << sigstr << ")";
        abnormal_program_termination(os.str().c_str());
    }

    GEO_NORETURN_DECL void fpe_signal_handler(
        int signal, siginfo_t* si, void* data
    ) GEO_NORETURN;
    
    void fpe_signal_handler(int signal, siginfo_t* si, void* data) {
        geo_argused(signal);
        geo_argused(data);
        const char* error;
        switch(si->si_code) {
            case FPE_INTDIV:
                error = "integer divide by zero";
                break;
            case FPE_INTOVF:
                error = "integer overflow";
                break;
            case FPE_FLTDIV:
                error = "floating point divide by zero";
                break;
            case FPE_FLTOVF:
                error = "floating point overflow";
                break;
            case FPE_FLTUND:
                error = "floating point underflow";
                break;
            case FPE_FLTRES:
                error = "floating point inexact result";
                break;
            case FPE_FLTINV:
                error = "floating point invalid operation";
                break;
            case FPE_FLTSUB:
                error = "subscript out of range";
                break;
            default:
                error = "unknown";
                break;
        }

        std::ostringstream os;
        os << "floating point exception detected: " << error;
        abnormal_program_termination(os.str().c_str());
    }

    void sigint_handler(int) {
        if(Progress::current_task() != nullptr) {
            Progress::cancel();
        } else {
            exit(1);
        }
    }

    GEO_NORETURN_DECL void terminate_handler() GEO_NORETURN;
    
    void terminate_handler() {
        abnormal_program_termination("function terminate() was called");
    }

    GEO_NORETURN_DECL void memory_exhausted_handler() GEO_NORETURN;
    
    void memory_exhausted_handler() {
        abnormal_program_termination("memory exhausted");
    }
}



namespace GEO {

    namespace Process {

        bool os_init_threads() {
#ifdef GEO_USE_PTHREAD_MANAGER
            Logger::out("Process")
                << "Using posix threads"
                << std::endl;
            set_thread_manager(new PThreadManager);
            return true;
#else
            return false;
#endif
        }

        void os_brute_force_kill() {
            kill(getpid(), SIGKILL);
        }

        index_t os_number_of_cores() {
#ifdef GEO_OS_ANDROID
            int nb_cores = android_get_number_of_cores();
            geo_assert(nb_cores > 0);
            return index_t(nb_cores);
#elif defined GEO_OS_EMSCRIPTEN
	    return 1;
#else	    
            return index_t(sysconf(_SC_NPROCESSORS_ONLN));
#endif
        }

        size_t os_used_memory() {
#ifdef GEO_OS_APPLE
            size_t result = 0;
            struct rusage usage;
            if(0 == getrusage(RUSAGE_SELF, &usage)) {
                result = (size_t) usage.ru_maxrss;
            }
            return result;
#else
            // The following method seems to be more 
            // reliable than  getrusage() under Linux.
            // It works for both Linux and Android.
            size_t result = 0;
            LineInput in("/proc/self/status");
            while(!in.eof() && in.get_line()) {
                in.get_fields();
                if(in.field_matches(0,"VmSize:")) {
                        result = size_t(in.field_as_uint(1)) * size_t(1024);
                    break;
                }
            }
            return result;
#endif
        }

        size_t os_max_used_memory() {
            // The following method seems to be more 
            // reliable than  getrusage() under Linux.
            // It works for both Linux and Android.
            size_t result = 0;
            LineInput in("/proc/self/status");
            
            // Some versions of Unix may not have the proc
            // filesystem (or a different organization)
            if(!in.OK()) {
                return result;
            }
            
            while(!in.eof() && in.get_line()) {
                in.get_fields();
                if(in.field_matches(0,"VmPeak:")) {
                    result = size_t(in.field_as_uint(1)) * size_t(1024);
                    break;
                }
            }
            return result;
        }

        bool os_enable_FPE(bool flag) {
#ifdef GEO_OS_APPLE
           unsigned int excepts = 0
                // | _MM_MASK_INEXACT   // inexact result
                   | _MM_MASK_DIV_ZERO  // division by zero
                   | _MM_MASK_UNDERFLOW // result not representable due to underflow
                   | _MM_MASK_OVERFLOW  // result not representable due to overflow
                   | _MM_MASK_INVALID   // invalid operation
                   ;
            // _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~excepts);
            geo_argused(flag);
            geo_argused(excepts);
            return true;
#else
            int excepts = 0
                // | FE_INEXACT   // inexact result
                   | FE_DIVBYZERO   // division by zero
                   | FE_UNDERFLOW // result not representable due to underflow
                   | FE_OVERFLOW    // result not representable due to overflow
                   | FE_INVALID     // invalid operation
                   ;
#ifdef GEO_OS_EMSCRIPTEN
            geo_argused(flag);
            geo_argused(excepts);
#else            
            if(flag) {
                feenableexcept(excepts);
            } else {
                fedisableexcept(excepts);
            }
#endif            
            return true;
#endif
        }

        bool os_enable_cancel(bool flag) {
            if(flag) {
                signal(SIGINT, sigint_handler);
            } else {
                signal(SIGINT, SIG_DFL);
            }
            return true;
        }

        void os_install_signal_handlers() {

            // Install signal handlers
            signal(SIGSEGV, signal_handler);
            signal(SIGILL, signal_handler);
            signal(SIGBUS, signal_handler);

            // Use sigaction for SIGFPE as it provides more details 
            // about the error.
            struct sigaction sa, old_sa;
            sa.sa_flags = SA_SIGINFO;
            sa.sa_sigaction = fpe_signal_handler;
            sigemptyset(&sa.sa_mask);
            sigaction(SIGFPE, &sa, &old_sa);

            // Install uncaught c++ exception handlers
            std::set_terminate(terminate_handler);

            // Install memory allocation handler
            std::set_new_handler(memory_exhausted_handler);
        }


        std::string os_executable_filename() {
            char buff[PATH_MAX];
#ifdef GEO_OS_APPLE
            uint32_t len=PATH_MAX;
            if (_NSGetExecutablePath(buff, &len) == 0) {
                std::string filename(buff);
                size_t pos = std::string::npos;
                while( (pos=filename.find("/./")) != std::string::npos ) {
                    filename.replace(pos, 3, "/");
                }
                return filename;
            }
            return std::string("");
#else
            ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
            if (len != -1) {
                buff[len] = '\0';
                return std::string(buff);
            }
            return std::string("");
#endif
        }        
        
    }
}

#else 

// Declare a dummy variable so that
// MSVC does not complain that it 
// generated an empty object file.
int dummy_process_unix_compiled = 1;

#endif


/******* extracted from ../basic/process_win.cpp *******/


#ifdef GEO_OS_WINDOWS


#include <sstream>
#include <windows.h>
#include <signal.h>
#include <new.h>
#include <rtcapi.h>
#include <psapi.h>

// MSVC++ 11.0 _MSC_VER = 1700  (2012)
// MSVC++ 10.0 _MSC_VER = 1600  (2010)
// MSVC++ 9.0  _MSC_VER = 1500  (2008)
// MSVC++ 8.0  _MSC_VER = 1400
// MSVC++ 7.1  _MSC_VER = 1310
// MSVC++ 7.0  _MSC_VER = 1300
// MSVC++ 6.0  _MSC_VER = 1200
// MSVC++ 5.0  _MSC_VER = 1100
//
// Thread pools are supported since Windows Vista (_WIN32_WINNT >= 0x600)
// See http://msdn.microsoft.com/fr-fr/library/aa383745.aspx
// for details on _WIN32_WINNT values.

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x600)
// In addition, I deactivate support of thread pool
// for Visual C++ <= 2008 (not mandatory, but else
// we get runtime errors on Windows XP)
#if (_MSC_VER > 1500)
#define GEO_OS_WINDOWS_HAS_THREADPOOL
#endif
#endif

namespace {

    using namespace GEO;

    class WindowsThreadManager : public ThreadManager {
    public:
        WindowsThreadManager() {
            InitializeCriticalSection(&lock_);
        }

        
	index_t maximum_concurrent_threads() override {
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            return sysinfo.dwNumberOfProcessors;
        }

        
	void enter_critical_section() override {
            EnterCriticalSection(&lock_);
        }

        
	void leave_critical_section() override {
            LeaveCriticalSection(&lock_);
        }

    protected:
        
	~WindowsThreadManager() override {
            DeleteCriticalSection(&lock_);
        }

        
	void run_concurrent_threads(
            ThreadGroup& threads, index_t max_threads
        ) override {
            // TODO: take max_threads into account
            geo_argused(max_threads);

            HANDLE* threadsHandle = new HANDLE[threads.size()];
            for(index_t i = 0; i < threads.size(); i++) {
                set_thread_id(threads[i],i);
                threadsHandle[i] = CreateThread(
                    nullptr, 0, run_thread, (void*) &threads[i], 0, nullptr
                );
            }
            WaitForMultipleObjects(
                DWORD(threads.size()), threadsHandle, TRUE, INFINITE
            );
            for(index_t i = 0; i < threads.size(); i++) {
                CloseHandle(threadsHandle[i]);
            }
            delete[] threadsHandle;
        }

        static DWORD WINAPI run_thread(LPVOID p) {
            Thread* thread = (*reinterpret_cast<Thread_var*>(p));
            // Sets the thread-local-storage instance pointer, so
            // that Thread::current() can retrieve it.
            set_current_thread(thread);
            thread->run();
            return 0;
        }

    private:
        CRITICAL_SECTION lock_;
    };

#ifdef GEO_OS_WINDOWS_HAS_THREADPOOL

    class WindowsThreadPoolManager : public WindowsThreadManager {
    public:
        WindowsThreadPoolManager() {
            pool_ = CreateThreadpool(nullptr);
            InitializeThreadpoolEnvironment(&cbe_);
            cleanupGroup_ = CreateThreadpoolCleanupGroup();
            SetThreadpoolCallbackPool(&cbe_, pool_);
            SetThreadpoolCallbackCleanupGroup(&cbe_, cleanupGroup_, nullptr);
            // Rem: cannot do what follows, since
            // maximum_concurrent_threads is not initialized yet...
            // SetThreadpoolThreadMaximum(
            //   pool_, Process::maximum_concurrent_threads()
            // );
            // SetThreadpoolThreadMinimum(
            //   pool_, Process::maximum_concurrent_threads()
            // );
            threadCounter_ = 0;
        }

    protected:
        
	~WindowsThreadPoolManager() override {
// It makes it crash on exit when calling these functions
// with dynamic libs, I do not know why...            
// TODO: investigate...
#ifndef GEO_DYNAMIC_LIBS            
            CloseThreadpool(pool_);
            CloseThreadpoolCleanupGroup(cleanupGroup_);
#endif            
        }

        
	void run_concurrent_threads(
            ThreadGroup& threads, index_t max_threads
        ) override {
            // TODO: take max_threads into account
            geo_argused(max_threads);

            // geo_assert(!Process::is_running_threads());
            // --> no, this doesn't work
            TP_WORK* worker = CreateThreadpoolWork(
                run_thread, (void*) &threads, &cbe_
            );
            threadCounter_ = 0;
            for(index_t i = 0; i < threads.size(); i++) {
                SubmitThreadpoolWork(worker);
            }
            WaitForThreadpoolWorkCallbacks(worker, FALSE);
            CloseThreadpoolWork(worker);
        }

        static VOID CALLBACK run_thread(
            PTP_CALLBACK_INSTANCE Instance,
            PVOID Context,
            PTP_WORK Work
        ) {
            geo_argused(Work);
            geo_argused(Instance);
            ThreadGroup& threads = *reinterpret_cast<ThreadGroup*>(Context);
            LONG id = InterlockedIncrement(&threadCounter_);
            DWORD tid = (id - 1) % threads.size();
            set_thread_id(threads[tid], tid);
            // Sets the thread-local-storage instance pointer, so
            // that Thread::current() can retreive it.
            set_current_thread(threads[tid]);
            threads[tid]->run();
        }

    private:
        PTP_POOL pool_;
        TP_CALLBACK_ENVIRON cbe_;
        PTP_CLEANUP_GROUP cleanupGroup_;
        static volatile LONG threadCounter_;
    };

    volatile long WindowsThreadPoolManager::threadCounter_ = 0;

#endif

#ifdef GEO_COMPILER_MSVC    
    void abnormal_program_termination(const char* message = nullptr) {
        if(message != nullptr) {
            // Do not use Logger here!
            std::cout
                << "Abnormal program termination: "
                << message << std::endl;
        }
        ExitProcess(1);
    }

    void signal_handler(int signal) {
        const char* sigstr;
        switch(signal) {
            case SIGINT:
                sigstr = "SIGINT";
                break;
            case SIGILL:
                sigstr = "SIGILL";
                break;
            case SIGFPE:
                sigstr = "SIGFPE";
                break;
            case SIGSEGV:
                sigstr = "SIGSEGV";
                break;
            case SIGTERM:
                sigstr = "SIGTERM";
                break;
            case SIGBREAK:
                sigstr = "SIGBREAK";
                break;
            case SIGABRT:
                sigstr = "SIGABRT";
                break;
            default:
                sigstr = "UNKNOWN";
                break;
        }

        std::ostringstream os;
        os << "received signal " << signal << " (" << sigstr << ")";
        abnormal_program_termination(os.str().c_str());
    }

    void fpe_signal_handler(int /*signal*/, int code) {
        const char* error;
        switch(code) {
            case _FPE_INVALID:
                error = "invalid number";
                break;
            case _FPE_OVERFLOW:
                error = "overflow";
                break;
            case _FPE_UNDERFLOW:
                error = "underflow";
                break;
            case _FPE_ZERODIVIDE:
                error = "divide by zero";
                break;
            default:
                error = "unknown";
                break;
        }

        std::ostringstream os;
        os << "floating point exception detected: " << error;
        abnormal_program_termination(os.str().c_str());
    }

    void sigint_handler(int) {
        if(Progress::current_task() != nullptr) {
            Progress::cancel();
        } else {
            exit(1);
        }
    }

    void uncaught_exception_handler() {
        abnormal_program_termination("function terminate() was called");
    }

// Disable the "unreachable code" warning issued by
// Microsoft Visual C++
// (abnormal_program_termination() does not return,
//  but memory_exhausted_handler() needs to return
//  something...)    
#ifdef GEO_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4702)
#endif
    
    int memory_exhausted_handler(size_t) {
        abnormal_program_termination("memory exhausted");
        return 0; 
    }

#ifdef GEO_COMPILER_MSVC    
#pragma warning(pop)
#endif
    
    void pure_call_handler() {
        abnormal_program_termination("pure virtual function called");
    }

    void invalid_parameter_handler(
        const wchar_t* expression,
        const wchar_t* function,
        const wchar_t* file,
        unsigned int line,
        uintptr_t /*pReserved*/
    ) {
        wprintf(
            L"Abnormal program termination: Invalid parameter detected.\n"
            L"Function: %s\nFile: %s\nLine: %d\n",
            function, file, int(line)
        );
        wprintf(L"Expression: %s\n", expression);
        abnormal_program_termination();
    }

    int runtime_error_handler(
        int /*errorType*/,
        const wchar_t* filename,
        int linenumber,
        const wchar_t* moduleName,
        const wchar_t* format,
        ...
    ) {
        va_list vl;
        va_start(vl, format);

        wprintf(L"Abnormal program termination: ");
        vwprintf(format, vl);
        wprintf(
            L"\nModule: %s\nFile: %s\nLine: %d\n", 
            moduleName, filename, linenumber

        );
        va_end(vl);
        
        // Must return 1 to force the program to stop with an exception which
        // will be captured by the unhandled exception handler
        return 1;
    }

    int debug_report_hook(int reportType, char* message, int* returnValue) {
        if(reportType != _CRT_WARN) {
            // Critical error: exit the application
            abnormal_program_termination(message);
        }

        // Runtime warning messages        
        if(Logger::is_initialized()) {
            Logger::err("SignalHandler") << message << std::endl;
        } else {
            fprintf(stderr, "SignalHandler: %s\n", message);
        }

        // Tell _CrtDbgReport to continue processing
        if(returnValue != 0) {
            *returnValue = 0;
        }

        // Tell _CrtDbgReport that no further reporting is required.
        return TRUE;
    }
    
#endif
    
}



namespace GEO {

    namespace Process {

        bool os_init_threads() {
#ifdef GEO_COMPILER_MSVC
#  ifdef GEO_OS_WINDOWS_HAS_THREADPOOL
            // Env. variable to deactivate thread pool, e.g.
            // used under Wine (that does not implement thread pools yet).
            if(::getenv("GEO_NO_THREAD_POOL")) {
                Logger::out("Process")
                    << "Windows thread pool disabled by GEO_NO_THREAD_POOL"
                    << ", using Windows threads"
                    << std::endl;
                set_thread_manager(new WindowsThreadManager);
            } else {
                Logger::out("Process")
                    << "Using Windows thread pool"
                    << std::endl;
                set_thread_manager(new WindowsThreadPoolManager);
            }
            return true;
#  else
            Logger::out("Process")
                << "Windows thread pool not supported, using Windows threads"
                << std::endl;
            set_thread_manager(new WindowsThreadManager);
            return true;
#  endif
#else
	   // If compiling for Windows with a compiler different from MSVC, 
	   // return false, and use OpenMP fallback from process.cpp
	   return false;
#endif	   
        }

        void os_brute_force_kill() {
            // Get the pid of this process
            DWORD processId = GetCurrentProcessId();

            // then modify its privileges to allow full access
            HANDLE hHandle;

            hHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION, 0, processId);
            HANDLE tokHandle;
            OpenProcessToken(hHandle, TOKEN_ALL_ACCESS, &tokHandle);

            TOKEN_PRIVILEGES tp;
            LUID luid;
            LookupPrivilegeValue(
                nullptr,            // lookup privilege on local system
                SE_DEBUG_NAME,   // privilege to lookup
                &luid);

            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            // Enable the privilege.

            AdjustTokenPrivileges(
                tokHandle,
                FALSE,
                &tp,
                sizeof(TOKEN_PRIVILEGES),
                (PTOKEN_PRIVILEGES) nullptr,
                (PDWORD) nullptr
            );

            if(hHandle == nullptr) {
                DWORD err = GetLastError();
                geo_argused(err);
            }

            // kill the process in a quite brutal way...
            HANDLE hHandle2 = ::OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
            DWORD dwExitCode = 0;
            // we don't need to know the current state of the process :
            //   it is STILL_ACTIVE (259)
            // and we want this termination to look normal (exit with code 0)
            // ::GetExitCodeProcess(hHandle2,&dwExitCode);
            ::TerminateProcess(hHandle2, dwExitCode);
        }

        index_t os_number_of_cores() {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            return si.dwNumberOfProcessors;
        }

        size_t os_used_memory() {
#ifdef GEO_COMPILER_MSVC
            PROCESS_MEMORY_COUNTERS info;
#if (PSAPI_VERSION >= 2)
            K32GetProcessMemoryInfo(GetCurrentProcess( ), &info, sizeof(info));            
#else            
            GetProcessMemoryInfo(GetCurrentProcess( ), &info, sizeof(info));
#endif            
            return size_t(info.WorkingSetSize);
#else
	   return size_t(0);
#endif	   
        }

        size_t os_max_used_memory() {
#ifdef GEO_COMPILER_MSVC	   
            PROCESS_MEMORY_COUNTERS info;
#if (PSAPI_VERSION >= 2)
            K32GetProcessMemoryInfo(GetCurrentProcess( ), &info, sizeof(info));            
#else            
            GetProcessMemoryInfo(GetCurrentProcess( ), &info, sizeof(info));
#endif            
            return size_t(info.PeakWorkingSetSize);
#else
	   return size_t(0);
#endif	   
        }

        bool os_enable_FPE(bool flag) {
#ifdef GEO_COMPILER_MSVC	    
            if(flag) {
                unsigned int excepts = 0
                    // | _EM_INEXACT // inexact result
                    | _EM_ZERODIVIDE // division by zero
                    | _EM_UNDERFLOW // result not representable due to underflow
                    | _EM_OVERFLOW  // result not representable due to overflow
                    | _EM_INVALID   // invalid operation
                ;
                _clearfp();
                _controlfp(~excepts, _MCW_EM);
            } else {
                _controlfp(_MCW_EM, _MCW_EM);
            }
            return true;
#else
	    geo_argused(flag);
	    return false;
#endif	    
        }

        bool os_enable_cancel(bool flag) {
#ifdef GEO_COMPILER_MSVC	    
            if(flag) {
                signal(SIGINT, sigint_handler);
            } else {
                signal(SIGINT, SIG_DFL);
            }
            return true;
#else
	    geo_argused(flag);
	    return false;
#endif	    
        }

#ifdef GEO_COMPILER_MSVC	
        void os_install_signal_handlers() {

            // Install signal handlers
            signal(SIGSEGV, signal_handler);
            signal(SIGILL, signal_handler);
            signal(SIGBREAK, signal_handler);
            signal(SIGTERM, signal_handler);

            // SIGFPE has a dedicated handler 
            // that provides more details about the error.
            typedef void (__cdecl * sighandler_t)(int);
            signal(SIGFPE, (sighandler_t) fpe_signal_handler);

            // Install uncaught c++ exception handlers	    
            std::set_terminate(uncaught_exception_handler);

            // Install memory allocation handler
            _set_new_handler(memory_exhausted_handler);
            // Also catch malloc errors
            _set_new_mode(1);

            // Install Windows runtime error handlers.
            // This code and the above is inspired from a very good article
            // "Effective Exception Handling in Visual C++" available here:
            // http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus

            // Catch calls to pure virtual functions
            _set_purecall_handler(pure_call_handler);

            // Catch abort and assertion failures
            // By default abort() error messages are sent to a dialog box
            // which blocks the application. This is a very annoying behavior
            // especially during test sessions.
            // -> Redirect abort() messages to standard error.
            signal(SIGABRT, signal_handler);
            _set_abort_behavior(0, _WRITE_ABORT_MSG);

            // Catch "invalid parameter" runtime assertions
            _set_invalid_parameter_handler(invalid_parameter_handler);

            // Catch runtime check errors
            _RTC_SetErrorFuncW(runtime_error_handler);

            // Some debug runtime errors are not caught by the error handlers
            // installed above. We must install a custom report hook called by
            // _CrtDbgReport that prints the error message, print the stack
            // trace and exit the application.
            // NOTE: when this hook is installed, it takes precedence over the
            // invalid_parameter_handler(), but not over the 
            // runtime_error_handler().
            // Windows error handling is a nightmare!
            _CrtSetReportHook(debug_report_hook);

            // By default runtime error messages are sent to a dialog box
            // which blocks the application. This is a very annoying behavior
            // especially during test sessions.
            // -> Redirect runtime messages to standard error by security
            _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
            _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
            _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        }
#else
        void os_install_signal_handlers() {
	}
#endif	
	
        std::string os_executable_filename() {
            TCHAR result[MAX_PATH];
            GetModuleFileName( nullptr, result, MAX_PATH);
            return std::string(result);
        }
    }
}

#endif


/******* extracted from ../basic/factory.cpp *******/


namespace {

    using namespace GEO;

    typedef SmartPointer<InstanceRepo::Instance> Instance_var;

    typedef std::map<std::string, Instance_var> Registry;

    Registry& get_registry() {
        static Registry r;
        return r;
    }
}

namespace GEO {

    void InstanceRepo::add(const std::string& name, Instance* instance) {
        Registry& r = get_registry();
        r[name] = instance;
    }

    InstanceRepo::Instance* InstanceRepo::get(const std::string& name) {
        const Registry& r = get_registry();
        auto i = r.find(name);
        return i == r.end() ? nullptr : i->second.get();
    }
}


/******* extracted from ../basic/assert.cpp *******/

#include <stdlib.h>
#include <sstream>
#include <stdexcept>

#ifdef GEO_OS_WINDOWS
#include <intrin.h> // For __debugbreak()
#else
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef GEO_OS_ANDROID
#ifndef GEO_OS_EMSCRIPTEN
#include <execinfo.h>
#endif
#endif

#endif

namespace GEO {

    namespace {
#ifdef GEO_DEBUG
        AssertMode assert_mode_ = ASSERT_ABORT;        
#else
        AssertMode assert_mode_ = ASSERT_THROW;
#endif        
        bool aborting = false;
    }

    void set_assert_mode(AssertMode mode) {
        assert_mode_ = mode;
    }

    AssertMode assert_mode() {
        return assert_mode_;
    }

    void geo_abort() {
        // Avoid assert in assert !!
        if(aborting) {
            Process::brute_force_kill();
        }
        aborting = true;
        abort();
    }

    void geo_breakpoint() {
#ifdef GEO_COMPILER_MSVC
	__debugbreak();
#else
	geo_abort();
#endif	
    }
    
    void geo_assertion_failed(
        const std::string& condition_string,
        const std::string& file, int line
    ) {
        std::ostringstream os;
        os << "Assertion failed: " << condition_string << ".\n";
        os << "File: " << file << ",\n";
        os << "Line: " << line;

        if(assert_mode_ == ASSERT_THROW) {
	    if(Logger::instance()->is_quiet()) {
		std::cerr << os.str()
			  << std::endl;
	    }
	    throw std::runtime_error(os.str());
        } else if(assert_mode_ == ASSERT_ABORT) {
            Logger::err("Assert") << os.str() << std::endl;
            geo_abort();
        } else {
            Logger::err("Assert") << os.str() << std::endl;
	    geo_breakpoint();
	}
    }

    void geo_range_assertion_failed(
        double value, double min_value, double max_value,
        const std::string& file, int line
    ) {
        std::ostringstream os;
        os << "Range assertion failed: " << value
            << " in [ " << min_value << " ... " << max_value << " ].\n";
        os << "File: " << file << ",\n";
        os << "Line: " << line;

        if(assert_mode_ == ASSERT_THROW) {
            if(Logger::instance()->is_quiet()) {
                std::cerr << os.str()
                          << std::endl;
            }
            throw std::runtime_error(os.str());
        } else {
            Logger::err("Assert") << os.str() << std::endl;
            geo_abort();
        }
    }

    void geo_should_not_have_reached(
        const std::string& file, int line
    ) {
        std::ostringstream os;
        os << "Control should not have reached this point.\n";
        os << "File: " << file << ",\n";
        os << "Line: " << line;

        if(assert_mode_ == ASSERT_THROW) {
            if(Logger::instance()->is_quiet()) {
                std::cerr << os.str()
                          << std::endl;
            }
            throw std::runtime_error(os.str());
        } else {
            Logger::err("Assert") << os.str() << std::endl;
            geo_abort();
        }
    }
}


/******* extracted from ../basic/stopwatch.cpp *******/

#include <iostream>

#ifdef GEO_OS_EMPSCRITEN
#include <empscripten.h>
#endif

namespace GEO {

    

    SystemStopwatch::SystemStopwatch() {
#if defined(GEO_OS_WINDOWS)
        start_ = long(GetTickCount());
#elif defined(GEO_OS_EMSCRIPTEN)
        startf_ = now();
#else
        clock_t init_user = times(&start_);
        while((start_user_ = times(&start_)) == init_user) {
        }
#endif
    }

    double SystemStopwatch::elapsed_user_time() const {
#if defined(GEO_OS_WINDOWS)
        return double(long(GetTickCount()) - start_) / 1000.0;
#elif defined(GEO_OS_EMSCRIPTEN)
        return now() - startf_;
#else        
        clock_t end_user;
        tms end;
        end_user = times(&end);
        return double(end_user - start_user_) / 100.0;
#endif
    }

    double SystemStopwatch::now() {
#if defined(GEO_OS_WINDOWS)
        return double(GetTickCount()) / 1000.0;
#elif defined(GEO_OS_EMSCRIPTEN)
        // times() hangs on Emscripten but
        // clock() works. TODO: check with emscripten_get_now();        
        return double(clock()) / double(CLOCKS_PER_SEC);
#else
        tms now_tms;
        return double(times(&now_tms)) / 100.0;
#endif
    }

    void SystemStopwatch::print_elapsed_time(std::ostream& os) const {
#if defined(GEO_OS_WINDOWS)
        os << "---- Times (seconds) ----"
            << "\n  Elapsed time: " << elapsed_user_time()
            << std::endl;
#elif defined(GEO_OS_EMSCRIPTEN)
        os << "---- Times (seconds) ----"
            << "\n  Elapsed time: " << elapsed_user_time()
            << std::endl;
#else
        clock_t end_user;
        tms end;
        end_user = times(&end);

        os << "---- Times (seconds) ----"
            << "\n  Real time: "
            << double(end_user - start_user_) / 100.0

    << "\n  User time: "
    << double(end.tms_utime - start_.tms_utime) / 100.0

    << "\n  Syst time: "
    << double(end.tms_stime - start_.tms_stime) / 100.0
    << std::endl;
#endif
    }

    

    // For now, uses SystemStopwatch,
    // TODO: If need be, reimplement with ASM RDTSC instruction
    // (read processor timer).
    Numeric::uint64 ProcessorStopwatch::now() {
        return Numeric::uint64(SystemStopwatch::now() * 1000.0);
    }

    
}


/******* extracted from ../basic/numeric.cpp *******/

#include <stdlib.h>

#ifdef GEO_COMPILER_EMSCRIPTEN
#pragma GCC diagnostic ignored "-Wc++11-long-long"
#endif

namespace GEO {

    namespace Numeric {

        bool is_nan(float32 x) {
#ifdef GEO_COMPILER_MSVC
            return _isnan(x) || !_finite(x);	    
#else	    
            return std::isnan(x) || !std::isfinite(x);
#endif	    
        }

        bool is_nan(float64 x) {
#ifdef GEO_COMPILER_MSVC
            return _isnan(x) || !_finite(x);	    	    
#else	    
            return std::isnan(x) || !std::isfinite(x);
#endif	    
        }

        void random_reset() {
#ifdef GEO_OS_WINDOWS
            srand(1);
#else
            srandom(1);
#endif
        }

        int32 random_int32() {
#ifdef GEO_OS_WINDOWS
            return rand();
#else
            return int32(random() % std::numeric_limits<int32>::max());
#endif
        }

        float32 random_float32() {
#if defined(GEO_OS_WINDOWS)
            return float(rand()) / float(RAND_MAX);
#elif defined(GEO_OS_ANDROID)
            // TODO: find a way to call drand48()
            // (problem at link time)
            return
                float(random_int32()) /
                float(std::numeric_limits<int32>::max());
#else
            return float(drand48());
#endif
        }

        float64 random_float64() {
#if defined(GEO_OS_WINDOWS)
            return double(rand()) / double(RAND_MAX);
#elif defined(GEO_OS_ANDROID)
            // TODO: find a way to call drand48()
            // (problem at link time)
            return
                double(random_int32()) /
                double(std::numeric_limits<int32>::max());
#else
            return double(drand48());
#endif
        }
    }
}


/******* extracted from ../mesh/mesh_reorder.h *******/

#ifndef GEOGRAM_MESH_MESH_REORDER
#define GEOGRAM_MESH_MESH_REORDER



namespace GEO {


#ifndef GEOGRAM_PSM     
    class Mesh;

    enum MeshOrder {
        MESH_ORDER_HILBERT,
        MESH_ORDER_MORTON
    };

    void GEOGRAM_API mesh_reorder(
        Mesh& M, MeshOrder order = MESH_ORDER_HILBERT
    );

#endif
    
    void GEOGRAM_API compute_Hilbert_order(
        index_t nb_vertices, const double* vertices,
        vector<index_t>& sorted_indices,
        index_t stride = 3
    );


    void GEOGRAM_API compute_Hilbert_order(
        index_t total_nb_vertices, const double* vertices,
        vector<index_t>& sorted_indices,
        index_t first,
        index_t last,
        index_t dimension, index_t stride = 3
    );
    
    void GEOGRAM_API compute_BRIO_order(
        index_t nb_vertices, const double* vertices,
        vector<index_t>& sorted_indices,
	index_t dimension,
        index_t stride = 3,
        index_t threshold = 64,
        double ratio = 0.125,
        vector<index_t>* levels = nullptr
    );

    void GEOGRAM_API Hilbert_sort_periodic(
	index_t nb_vertices, const double* vertices,
	vector<index_t>& sorted_indices,
	index_t dimension,
        index_t stride,
	vector<index_t>::iterator first,
	vector<index_t>::iterator last,
	double period = 1.0
    );

    
    
}

#endif


/******* extracted from ../mesh/mesh_reorder.cpp *******/


#include <random>

namespace {

    using namespace GEO;

    template <class IT, class CMP>
    inline IT reorder_split(
        IT begin, IT end, CMP cmp
    ) {
        if(begin >= end) {
            return begin;
        }
        IT middle = begin + (end - begin) / 2;
        std::nth_element(begin, middle, end, cmp);
        return middle;
    }

    

    class VertexArray {
    public:

        VertexArray(
            index_t nb_vertices,
            const double* base, index_t stride
        ) :
            base_(base),
            stride_(stride) {
            nb_vertices_ = nb_vertices;
        }

        const double* point_ptr(index_t i) const {
            geo_debug_assert(i < nb_vertices_);
            return base_ + i * stride_;
        }

    private:
        const double* base_;
        index_t stride_;
        index_t nb_vertices_;
    };


    class VertexMesh {
    public:
        VertexMesh(
            index_t nb_vertices,
            const double* base, index_t stride
        ) : vertices(nb_vertices, base, stride) {
        }
        VertexArray vertices;
    };
    
    

    template <int COORD, bool UP, class MESH>
    struct Hilbert_vcmp {
    };

    template <int COORD, class MESH>
    struct Hilbert_vcmp<COORD, true, MESH> {

        Hilbert_vcmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        bool operator() (index_t i1, index_t i2) {
            return
                mesh_.vertices.point_ptr(i1)[COORD] <
                mesh_.vertices.point_ptr(i2)[COORD];
        }

        const MESH& mesh_;
    };

    template <int COORD, class MESH>
    struct Hilbert_vcmp<COORD, false, MESH> {

        Hilbert_vcmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        bool operator() (index_t i1, index_t i2) {
            return
                mesh_.vertices.point_ptr(i1)[COORD] >
                mesh_.vertices.point_ptr(i2)[COORD];
        }

        const MESH& mesh_;
    };

    

    template <int COORD, bool UP, class MESH>
    struct Morton_vcmp {

        Morton_vcmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        bool operator() (index_t i1, index_t i2) {
            return
                mesh_.vertices.point_ptr(i1)[COORD] <
                mesh_.vertices.point_ptr(i2)[COORD];
        }

        const MESH& mesh_;
    };

    

#ifndef GEOGRAM_PSM
    
    template <int COORD, class MESH>
    class Base_fcmp {
    public:
        Base_fcmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        double center(index_t f) const {
            double result = 0.0;
            for(
                index_t c = mesh_.facets.corners_begin(f);
                c < mesh_.facets.corners_end(f); ++c
            ) {
                result += mesh_.vertices.point_ptr(
                    mesh_.facet_corners.vertex(c)
                )[COORD];
            }
            return result;
            // TODO: should be  / double(mesh_.facets.nb_vertices(f));
            // but this breaks one of the tests, to be investigated...
        }

    private:
        const MESH& mesh_;
    };

    template <int COORD, bool UP, class MESH>
    struct Hilbert_fcmp {
    };

    template <int COORD, class MESH>
    class Hilbert_fcmp<COORD, true, MESH> : public Base_fcmp<COORD, MESH> {
    public:
        Hilbert_fcmp(const MESH& mesh) :
            Base_fcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t f1, index_t f2) {
            return this->center(f1) < this->center(f2);
        }
    };

    template <int COORD, class MESH>
    class Hilbert_fcmp<COORD, false, MESH> : public Base_fcmp<COORD, MESH> {
    public:
        Hilbert_fcmp(const MESH& mesh) :
            Base_fcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t f1, index_t f2) {
            return this->center(f1) > this->center(f2);
        }
    };

    template <int COORD, bool UP, class MESH>
    class Morton_fcmp : public Base_fcmp<COORD, MESH> {
    public:
        Morton_fcmp(const MESH& mesh) :
            Base_fcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t f1, index_t f2) {
            return this->center(f1) < this->center(f2);
        }
    };

    

    template <int COORD, class MESH>
    class Base_tcmp {
    public:
        Base_tcmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        double center(index_t t) const {
            double result = 0.0;
            for(
                index_t lv = 0; lv < 4; ++lv
            ) {
                result += mesh_.vertices.point_ptr(
                    mesh_.cells.vertex(t, lv)
                )[COORD];
            }
            return result;
        }

    private:
        const MESH& mesh_;
    };

    template <int COORD, bool UP, class MESH>
    struct Hilbert_tcmp {
    };

    template <int COORD, class MESH>
    class Hilbert_tcmp<COORD, true, MESH> : public Base_tcmp<COORD, MESH> {
    public:
        Hilbert_tcmp(const MESH& mesh) :
            Base_tcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t t1, index_t t2) {
            return this->center(t1) < this->center(t2);
        }
    };

    template <int COORD, class MESH>
    class Hilbert_tcmp<COORD, false, MESH> : public Base_tcmp<COORD, MESH> {
    public:
        Hilbert_tcmp(const MESH& mesh) :
            Base_tcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t t1, index_t t2) {
            return this->center(t1) > this->center(t2);
        }
    };

    template <int COORD, bool UP, class MESH>
    class Morton_tcmp : public Base_tcmp<COORD, MESH> {
    public:
        Morton_tcmp(const MESH& mesh) :
            Base_tcmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t t1, index_t t2) {
            return this->center(t1) < this->center(t2);
        }
    };

    
    
    template <int COORD, class MESH>
    class Base_ccmp {
    public:
        Base_ccmp(const MESH& mesh) :
            mesh_(mesh) {
        }

        double center(index_t c) const {
            double result = 0.0;
            for(
                index_t lv = 0; lv < mesh_.cells.nb_vertices(c); ++lv
            ) {
                result += mesh_.vertices.point_ptr(
                    mesh_.cells.vertex(c, lv)
                )[COORD];
            }
            return result / double(mesh_.cells.nb_vertices(c));
        }

    private:
        const MESH& mesh_;
    };

    template <int COORD, bool UP, class MESH>
    struct Hilbert_ccmp {
    };

    template <int COORD, class MESH>
    class Hilbert_ccmp<COORD, true, MESH> : public Base_ccmp<COORD, MESH> {
    public:
        Hilbert_ccmp(const MESH& mesh) :
            Base_ccmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t c1, index_t c2) {
            return this->center(c1) < this->center(c2);
        }
    };

    template <int COORD, class MESH>
    class Hilbert_ccmp<COORD, false, MESH> : public Base_ccmp<COORD, MESH> {
    public:
        Hilbert_ccmp(const MESH& mesh) :
            Base_ccmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t c1, index_t c2) {
            return this->center(c1) > this->center(c2);
        }
    };

    template <int COORD, bool UP, class MESH>
    class Morton_ccmp : public Base_ccmp<COORD, MESH> {
    public:
        Morton_ccmp(const MESH& mesh) :
            Base_ccmp<COORD, MESH>(mesh) {
        }

        bool operator() (index_t c1, index_t c2) {
            return this->center(c1) < this->center(c2);
        }
    };

#endif
    
    
    
    template <template <int COORD, bool UP, class MESH> class CMP, class MESH>
    struct HilbertSort3d {

        template <int COORDX, bool UPX, bool UPY, bool UPZ, class IT>
        static void sort(
            const MESH& M, IT begin, IT end, index_t limit = 1
        ) {
            const int COORDY = (COORDX + 1) % 3, COORDZ = (COORDY + 1) % 3;
            if(end - begin <= signed_index_t(limit)) {
                return;
            }
            IT m0 = begin, m8 = end;
            IT m4 = reorder_split(m0, m8, CMP<COORDX, UPX, MESH>(M));
            IT m2 = reorder_split(m0, m4, CMP<COORDY, UPY, MESH>(M));
            IT m1 = reorder_split(m0, m2, CMP<COORDZ, UPZ, MESH>(M));
            IT m3 = reorder_split(m2, m4, CMP<COORDZ, !UPZ, MESH>(M));
            IT m6 = reorder_split(m4, m8, CMP<COORDY, !UPY, MESH>(M));
            IT m5 = reorder_split(m4, m6, CMP<COORDZ, UPZ, MESH>(M));
            IT m7 = reorder_split(m6, m8, CMP<COORDZ, !UPZ, MESH>(M));
            sort<COORDZ, UPZ, UPX, UPY>(M, m0, m1);
            sort<COORDY, UPY, UPZ, UPX>(M, m1, m2);
            sort<COORDY, UPY, UPZ, UPX>(M, m2, m3);
            sort<COORDX, UPX, !UPY, !UPZ>(M, m3, m4);
            sort<COORDX, UPX, !UPY, !UPZ>(M, m4, m5);
            sort<COORDY, !UPY, UPZ, !UPX>(M, m5, m6);
            sort<COORDY, !UPY, UPZ, !UPX>(M, m6, m7);
            sort<COORDZ, !UPZ, !UPX, UPY>(M, m7, m8);
        }

        HilbertSort3d(
            const MESH& M,
            vector<index_t>::iterator b,
            vector<index_t>::iterator e,
            index_t limit = 1
        ) :
            M_(M)
        {
            geo_debug_assert(e > b);
	    geo_cite_with_info(
		"WEB:SpatialSorting",
		"The implementation of spatial sort in GEOGRAM is inspired by "
		"the idea of using \\verb|std::nth_element()| and the recursive"
                " template in the spatial sort package of CGAL"
	    );

            // If the sequence is smaller than the limit, skip it
            if(index_t(e - b) <= limit) {
                return;
            }

            // If the sequence is smaller than 1024, use sequential sorting
            if(index_t(e - b) < 1024) {
                sort<0, false, false, false>(M_, b, e);
                return;
            }

            // Parallel sorting (2 then 4 then 8 sorts in parallel)

// Unfortunately we cannot access consts for template arguments in lambdas in all
// compilers (gcc is OK but not MSVC) so I'm using (ugly) macros here...
	    
#          define COORDX 0
#          define COORDY 1
#          define COORDZ 2
#          define UPX false
#          define UPY false
#          define UPZ false
	    
            m0_ = b;
            m8_ = e;
            m4_ = reorder_split(m0_, m8_, CMP<COORDX, UPX, MESH>(M));

	    
	    parallel(
		[this]() { m2_ = reorder_split(m0_, m4_, CMP<COORDY,  UPY, MESH>(M_)); },
		[this]() { m6_ = reorder_split(m4_, m8_, CMP<COORDY, !UPY, MESH>(M_)); }
	    );

	    parallel(
		[this]() { m1_ = reorder_split(m0_, m2_, CMP<COORDZ,  UPZ, MESH>(M_)); },
		[this]() { m3_ = reorder_split(m2_, m4_, CMP<COORDZ, !UPZ, MESH>(M_)); },
		[this]() { m5_ = reorder_split(m4_, m6_, CMP<COORDZ,  UPZ, MESH>(M_)); },
		[this]() { m7_ = reorder_split(m6_, m8_, CMP<COORDZ, !UPZ, MESH>(M_)); }
	    );

	    parallel(
		[this]() { sort<COORDZ,  UPZ,  UPX,  UPY>(M_, m0_, m1_); },
		[this]() { sort<COORDY,  UPY,  UPZ,  UPX>(M_, m1_, m2_); },
		[this]() { sort<COORDY,  UPY,  UPZ,  UPX>(M_, m2_, m3_); },
		[this]() { sort<COORDX,  UPX, !UPY, !UPZ>(M_, m3_, m4_); },
		[this]() { sort<COORDX,  UPX, !UPY, !UPZ>(M_, m4_, m5_); },
		[this]() { sort<COORDY, !UPY,  UPZ, !UPX>(M_, m5_, m6_); },
		[this]() { sort<COORDY, !UPY,  UPZ, !UPX>(M_, m6_, m7_); },
		[this]() { sort<COORDZ, !UPZ, !UPX,  UPY>(M_, m7_, m8_); }
	    );

#          undef COORDX
#          undef COORDY
#          undef COORDZ
#          undef UPX
#          undef UPY
#          undef UPZ	    
        }

    private:
        const MESH& M_;
        vector<index_t>::iterator
            m0_, m1_, m2_, m3_, m4_, m5_, m6_, m7_, m8_;
    };

    

    template <template <int COORD, bool UP, class MESH> class CMP, class MESH>
    struct HilbertSort2d {

        template <int COORDX, bool UPX, bool UPY, class IT>
        static void sort(
            const MESH& M, IT begin, IT end, index_t limit = 1
        ) {
            const int COORDY = (COORDX + 1) % 2;
            if(end - begin <= signed_index_t(limit)) {
                return;
            }
	    IT m0 = begin, m4 = end;

	    IT m2 = reorder_split (m0, m4, CMP<COORDX,  UPX, MESH>(M));
	    IT m1 = reorder_split (m0, m2, CMP<COORDY,  UPY, MESH>(M));
	    IT m3 = reorder_split (m2, m4, CMP<COORDY, !UPY, MESH>(M));

	    sort<COORDY, UPY, UPX> (M, m0, m1);
	    sort<COORDX, UPX, UPY> (M, m1, m2);
	    sort<COORDX, UPX, UPY> (M, m2, m3);
	    sort<COORDY,!UPY,!UPX> (M, m3, m4);
        }

        HilbertSort2d(
            const MESH& M,
            vector<index_t>::iterator b,
            vector<index_t>::iterator e,
            index_t limit = 1
        ) :
            M_(M)
        {
            geo_debug_assert(e > b);
	    geo_cite_with_info(
		"WEB:SpatialSorting",
		"The implementation of spatial sort in GEOGRAM is inspired by "
		"the idea of using \\verb|std::nth_element()| and the recursive"
                " template in the spatial sort package of CGAL"
	    );

            // If the sequence is smaller than the limit, skip it
            if(index_t(e - b) <= limit) {
                return;
            }
	    sort<0, false, false>(M_, b, e);
        }
    private:
        const MESH& M_;
    };

    

#ifndef GEOGRAM_PSM
    
    void hilbert_vsort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.vertices.nb());
        for(index_t i = 0; i < M.vertices.nb(); i++) {
            sorted_indices[i] = i;
        }
        HilbertSort3d<Hilbert_vcmp, Mesh>(
            M, sorted_indices.begin(), sorted_indices.end()
        );
    }

    void hilbert_fsort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.facets.nb());
        for(index_t i = 0; i < M.facets.nb(); i++) {
            sorted_indices[i] = i;
        }
        HilbertSort3d<Hilbert_fcmp, Mesh>(
            M, sorted_indices.begin(), sorted_indices.end()
        );
    }

    void hilbert_csort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.cells.nb());
        for(index_t i = 0; i < M.cells.nb(); i++) {
            sorted_indices[i] = i;
        }
        if(M.cells.are_simplices()) {
            HilbertSort3d<Hilbert_tcmp, Mesh>(
                M, sorted_indices.begin(), sorted_indices.end()
            );
        } else {
            HilbertSort3d<Hilbert_ccmp, Mesh>(
                M, sorted_indices.begin(), sorted_indices.end()
            );
        }
    }

    void morton_vsort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.vertices.nb());
        for(index_t i = 0; i < M.vertices.nb(); i++) {
            sorted_indices[i] = i;
        }
        HilbertSort3d<Morton_vcmp, Mesh>(
            M, sorted_indices.begin(), sorted_indices.end()
        );
    }

    void morton_fsort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.facets.nb());
        for(index_t i = 0; i < M.facets.nb(); i++) {
            sorted_indices[i] = i;
        }
        HilbertSort3d<Morton_fcmp, Mesh>(
            M, sorted_indices.begin(), sorted_indices.end()
        );
    }

    void morton_csort_3d(
        const Mesh& M, vector<index_t>& sorted_indices
    ) {
        sorted_indices.resize(M.cells.nb());
        for(index_t i = 0; i < M.cells.nb(); i++) {
            sorted_indices[i] = i;
        }
        if(M.cells.are_simplices()) {
            HilbertSort3d<Morton_tcmp, Mesh>(
                M, sorted_indices.begin(), sorted_indices.end()
            );
        } else {
            HilbertSort3d<Morton_ccmp, Mesh>(
                M, sorted_indices.begin(), sorted_indices.end()
            );
        }
    }

#endif    
    
    void compute_BRIO_order_recursive(
        index_t nb_vertices, const double* vertices,
        index_t dimension, index_t stride,
        vector<index_t>& sorted_indices,
        vector<index_t>::iterator b,
        vector<index_t>::iterator e,
        index_t threshold,
        double ratio,
        index_t& depth,
        vector<index_t>* levels
    ) {
        geo_debug_assert(e > b);

        vector<index_t>::iterator m = b;
        if(index_t(e - b) > threshold) {
            ++depth;
            m = b + int(double(e - b) * ratio);
            compute_BRIO_order_recursive(
                nb_vertices, vertices,
		dimension, stride,
                sorted_indices, b, m,
                threshold, ratio, depth,
                levels
            );
        }

        VertexMesh M(nb_vertices, vertices, stride);
	if(dimension == 3) {
	    HilbertSort3d<Hilbert_vcmp, VertexMesh>(
		M, m, e
	    );
	} else if(dimension ==2) {
	    HilbertSort2d<Hilbert_vcmp, VertexMesh>(
		M, m, e
	    );
	} else {
	    geo_assert_not_reached;
	}

        if(levels != nullptr) {
            levels->push_back(index_t(e - sorted_indices.begin()));
        }
    }
}



namespace GEO {

#ifndef GEOGRAM_PSM
    
    void mesh_reorder(Mesh& M, MeshOrder order) {

        geo_assert(M.vertices.dimension() >= 3);

        // Step 1: reorder vertices
        {
            vector<index_t> sorted_indices;
            switch(order) {
                case MESH_ORDER_HILBERT:
                    hilbert_vsort_3d(M, sorted_indices);
                    break;
                case MESH_ORDER_MORTON:
                    morton_vsort_3d(M, sorted_indices);
                    break;
            }
            M.vertices.permute_elements(sorted_indices);
        }

        // Step 2: reorder facets
        if(M.facets.nb() != 0) {
            vector<index_t> sorted_indices;
            switch(order) {
                case MESH_ORDER_HILBERT:
                    hilbert_fsort_3d(M, sorted_indices);
                    break;
                case MESH_ORDER_MORTON:
                    morton_fsort_3d(M, sorted_indices);
                    break;
            }
            M.facets.permute_elements(sorted_indices);
        }

        // Step 3: reorder cells
        if(M.cells.nb() != 0) {
            vector<index_t> sorted_indices;
            switch(order) {
                case MESH_ORDER_HILBERT:
                    hilbert_csort_3d(M, sorted_indices);
                    break;
                case MESH_ORDER_MORTON:
                    morton_csort_3d(M, sorted_indices);
                    break;
            }
            M.cells.permute_elements(sorted_indices);
        }
    }

#endif


    void compute_Hilbert_order(
        index_t total_nb_vertices, const double* vertices,
        vector<index_t>& sorted_indices,
        index_t first,
        index_t last,
        index_t dimension, index_t stride
    ) {
        geo_debug_assert(last > first);
        if(last - first <= 1) {
            return;
        }
        VertexMesh M(total_nb_vertices, vertices, stride);
	if(dimension == 3) {
	    HilbertSort3d<Hilbert_vcmp, VertexMesh>(
		M, sorted_indices.begin() + int(first),
		sorted_indices.begin() + int(last)
	    );
	} else if(dimension == 2) {
	    HilbertSort2d<Hilbert_vcmp, VertexMesh>(
		M, sorted_indices.begin() + int(first),
		sorted_indices.begin() + int(last)
	    );
	} else {
	    geo_assert_not_reached;
	}
    }
    
    void compute_BRIO_order(
        index_t nb_vertices, const double* vertices,
        vector<index_t>& sorted_indices,
	index_t dimension,
        index_t stride,
        index_t threshold,
        double ratio,
        vector<index_t>* levels
    ) {
        if(levels != nullptr) {
            levels->clear();
            levels->push_back(0);
        }
        index_t depth = 0;
        sorted_indices.resize(nb_vertices);
        for(index_t i = 0; i < nb_vertices; ++i) {
            sorted_indices[i] = i;
        }

        //The next three lines replace the following commented-out line
        //(random_shuffle is deprecated in C++17, and they call this 
        // progess...)
        //std::random_shuffle(sorted_indices.begin(), sorted_indices.end());       
        std::random_device rng;
        std::mt19937 urng(rng());
        std::shuffle(sorted_indices.begin(), sorted_indices.end(), urng);

        compute_BRIO_order_recursive(
            nb_vertices, vertices,
	    dimension, stride,
            sorted_indices,
            sorted_indices.begin(), sorted_indices.end(),
            threshold, ratio, depth, levels
        );
    }
}



namespace {
    using namespace GEO;

    // Same as in delaunay/periodic.cpp,
    // copied here for now because linker does not find
    // it under Android.
    int Periodic_translation[27][3] = {
	{  0,  0,  0}, //13 -> 0   +   <-- zero displacement is first.
	{ -1, -1, -1}, //0  -> 1   -
	{ -1, -1,  0}, //1  -> 2   -
	{ -1, -1,  1}, //2  -> 3   -
	{ -1,  0, -1}, //3  -> 4   -
	{ -1,  0,  0}, //4  -> 5   - 
	{ -1,  0,  1}, //5  -> 6   -
	{ -1,  1, -1}, //6  -> 7   -
	{ -1,  1,  0}, //7  -> 8   -
	{ -1,  1,  1}, //8  -> 9   - 
	{  0, -1, -1}, //9  -> 10  -
	{  0, -1,  0}, //10 -> 11  -
	{  0, -1,  1}, //11 -> 12  -
	{  0,  0, -1}, //12 -> 13  -
	// (zero displacement was there)
	{  0,  0,  1}, //14 -> 14  +
	{  0,  1, -1}, //15 -> 15  -
	{  0,  1,  0}, //16 -> 16  +
	{  0,  1,  1}, //17 -> 17  +
	{  1, -1, -1}, //18 -> 18  -
	{  1, -1,  0}, //19 -> 19  -
	{  1, -1,  1}, //20 -> 20  -
	{  1,  0, -1}, //21 -> 21  -
	{  1,  0,  0}, //22 -> 22  +
	{  1,  0,  1}, //23 -> 23  +
	{  1,  1, -1}, //24 -> 24  -
	{  1,  1,  0}, //25 -> 25  +
	{  1,  1,  1}  //26 -> 26  +
    };

    
    class PeriodicVertexArray3d {
    public:
        PeriodicVertexArray3d(
            index_t nb_vertices,
            const double* base, index_t stride,
	    double period = 1.0
        ) :
            base_(base),
            stride_(stride) {
            nb_vertices_ = nb_vertices;
	    nb_real_vertices_ = nb_vertices_ / 27;
	    geo_debug_assert(nb_vertices % 27 == 0);

	    
	    for(index_t i=0; i<27; ++i) {
		for(index_t j=0; j<3; ++j) {
		    xlat_[i][j] = period * double(Periodic_translation[i][j]);
		}
	    }
        }

	double point_coord(index_t i, index_t coord) const {
	    index_t instance = i / nb_real_vertices_;
	    i = i % nb_real_vertices_;
            return (base_ + i * stride_)[coord] + xlat_[instance][coord];
        }

    private:
        const double* base_;
        index_t stride_;
        index_t nb_vertices_;
	index_t nb_real_vertices_;
	double xlat_[27][3];
    };

    class PeriodicVertexMesh3d {
    public:
        PeriodicVertexMesh3d(
            index_t nb_vertices,
            const double* base, index_t stride, double period
	) : vertices(nb_vertices, base, stride, period) {
        }
	
        PeriodicVertexArray3d vertices;
    };

    template <int COORD, bool UP, class MESH>
    struct Hilbert_vcmp_periodic {
    };

    template <int COORD>
    struct Hilbert_vcmp_periodic<COORD, true, PeriodicVertexMesh3d> {
        Hilbert_vcmp_periodic(const PeriodicVertexMesh3d& mesh) :
            mesh_(mesh) {
        }
        bool operator() (index_t i1, index_t i2) {
            return
                mesh_.vertices.point_coord(i1,COORD) <
                mesh_.vertices.point_coord(i2,COORD);
        }
        const PeriodicVertexMesh3d& mesh_;
    };

    template <int COORD>
    struct Hilbert_vcmp_periodic<COORD, false, PeriodicVertexMesh3d> {
        Hilbert_vcmp_periodic(const PeriodicVertexMesh3d& mesh) :
            mesh_(mesh) {
        }
        bool operator() (index_t i1, index_t i2) {
            return
                mesh_.vertices.point_coord(i1,COORD) >
                mesh_.vertices.point_coord(i2,COORD);
        }
        const PeriodicVertexMesh3d& mesh_;
    };
    
}

namespace GEO {

    void Hilbert_sort_periodic(
	index_t nb_vertices, const double* vertices,
	vector<index_t>& sorted_indices,
	index_t dimension,
        index_t stride,
	vector<index_t>::iterator b,
	vector<index_t>::iterator e,
	double period
    ) {
	geo_assert(dimension == 3); // Only implemented for 3D.	
	geo_argused(sorted_indices); // Accessed through b and e.

       
        //The next three lines replace the following commented-out line
        //(random_shuffle is deprecated in C++17, and they call this 
        // progess...)
        // std::random_shuffle(b,e);
        std::random_device rng;
        std::mt19937 urng(rng());
        std::shuffle(b,e, urng);

	PeriodicVertexMesh3d M(nb_vertices, vertices, stride, period);
	HilbertSort3d<Hilbert_vcmp_periodic, PeriodicVertexMesh3d>(
	    M, b, e
	);
    }

}


/******* extracted from ../numerics/multi_precision.h *******/

#ifndef GEOGRAM_NUMERICS_MULTI_PRECISION
#define GEOGRAM_NUMERICS_MULTI_PRECISION

#include <iostream>
#include <new>


namespace GEO {

    extern double expansion_splitter_;
    extern double expansion_epsilon_;

    inline void two_sum(double a, double b, double& x, double& y) {
        x = a + b;
        double bvirt = x - a;
        double avirt = x - bvirt;
        double bround = b - bvirt;
        double around = a - avirt;
        y = around + bround;
    }

    inline void two_diff(double a, double b, double& x, double& y) {
        x = a - b;
        double bvirt = a - x;
        double avirt = x + bvirt;
        double bround = bvirt - b;
        double around = a - avirt;
        y = around + bround;
    }

    inline void split(double a, double& ahi, double& alo) {
        double c = expansion_splitter_ * a;
        double abig = c - a;
        ahi = c - abig;
        alo = a - ahi;
    }

    inline void two_product(double a, double b, double& x, double& y) {
#ifdef FP_FAST_FMA
        // If the target processor supports the FMA (Fused Multiply Add)
        // instruction, then the product of two doubles into a length-2
        // expansion can be implemented as follows. Thanks to Marc Glisse
        // for the information.
        // Note: under gcc, automatic generations of fma() for a*b+c needs
        // to be deactivated, using -ffp-contract=off, else it may break
        // other functions such as fast_expansion_sum_zeroelim().
        x = a*b;
        y = fma(a,b,-x);
#else
        x = a * b;
        double ahi, alo;
        split(a, ahi, alo);
        double bhi, blo;
        split(b, bhi, blo);
        double err1 = x - (ahi * bhi);
        double err2 = err1 - (alo * bhi);
        double err3 = err2 - (ahi * blo);
        y = (alo * blo) - err3;
#endif
    }

    inline void square(double a, double& x, double& y) {
#ifdef FP_FAST_FMA
        // If the target processor supports the FMA (Fused Multiply Add)
        // instruction, then the product of two doubles into a length-2
        // expansion can be implemented as follows. Thanks to Marc Glisse
        // for the information.
        // Note: under gcc, automatic generations of fma() for a*b+c needs
        // to be deactivated, using -ffp-contract=off, else it may break
        // other functions such as fast_expansion_sum_zeroelim().
        x = a*a;
        y = fma(a,a,-x);
#else
        x = a * a;
        double ahi, alo;
        split(a, ahi, alo);
        double err1 = x - (ahi * ahi);
        double err3 = err1 - ((ahi + ahi) * alo);
        y = (alo * alo) - err3;
#endif
    }

    

    class GEOGRAM_API expansion {
    public:
        index_t length() const {
            return length_;
        }

        index_t capacity() const {
            return capacity_;
        }

        void set_length(index_t new_length) {
            geo_debug_assert(new_length <= capacity());
            length_ = new_length;
        }

        const double& operator[] (index_t i) const {
            // Note: we allocate capacity+1 storage
            // systematically, since basic functions
            // may access one additional value (without
            // using it)
            geo_debug_assert(i <= capacity_);
            return x_[i];
        }

        double& operator[] (index_t i) {
            // Note: we allocate capacity+1 storage
            // systematically, since basic functions
            // may access one additional value (without
            // using it)
            geo_debug_assert(i <= capacity_);
            return x_[i];
        }

        double* data() {
            return x_;
        }

        const double* data() const {
            return x_;
        }

        static size_t bytes(index_t capa) {
            // --> 2*sizeof(double) because x_ is declared of size [2]
            // to avoid compiler's warning.
            // --> capa+1 to have an additional 'sentry' at the end
            // because fast_expansion_sum_zeroelim() may access
            // an entry past the end (without using it).
            return
                sizeof(expansion) - 2 * sizeof(double) +
                (capa + 1) * sizeof(double);
        }

        expansion(index_t capa) :
            length_(0),
            capacity_(capa) {
        }

#ifdef CPPCHECK
        // cppcheck does not understand that the result
        // of alloca() is passed to the placement syntax
        // of operator new.
    expansion& new_expansion_on_stack(index_t capa);         
#else
#define new_expansion_on_stack(capa)                           \
    (new (alloca(expansion::bytes(capa)))expansion(capa))
#endif

        static expansion* new_expansion_on_heap(index_t capa);

        static void delete_expansion_on_heap(expansion* e);

        // ========================== Initialization from doubles

	expansion& assign(double a) {
	    set_length(1);
	    x_[0] = a;
	    return *this;
	}
	
        static index_t sum_capacity(double a, double b) {
            geo_argused(a);
            geo_argused(b);
            return 2;
        }

        expansion& assign_sum(double a, double b) {
            set_length(2);
            two_sum(a, b, x_[1], x_[0]);
            return *this;
        }

        static index_t diff_capacity(double a, double b) {
            geo_argused(a);
            geo_argused(b);
            return 2;
        }

        expansion& assign_diff(double a, double b) {
            set_length(2);
            two_diff(a, b, x_[1], x_[0]);
            return *this;
        }

        static index_t product_capacity(double a, double b) {
            geo_argused(a);
            geo_argused(b);
            return 2;
        }

        expansion& assign_product(double a, double b) {
            set_length(2);
            two_product(a, b, x_[1], x_[0]);
            return *this;
        }

        static index_t square_capacity(double a) {
            geo_argused(a);
            return 2;
        }

        expansion& assign_square(double a) {
            set_length(2);
            square(a, x_[1], x_[0]);
            return *this;
        }

        // ====== Initialization from expansion and double

        static index_t sum_capacity(const expansion& a, double b) {
            geo_argused(b);
            return a.length() + 1;
        }

        expansion& assign_sum(const expansion& a, double b);

        static index_t diff_capacity(const expansion& a, double b) {
            geo_argused(b);
            return a.length() + 1;
        }

        expansion& assign_diff(const expansion& a, double b);

        static index_t product_capacity(const expansion& a, double b) {
            geo_argused(b);
            // TODO: implement special case where the double argument
            // is a power of two.
            return a.length() * 2;
        }

        expansion& assign_product(const expansion& a, double b);

        // ========================== Initialization from expansions

        static index_t sum_capacity(const expansion& a, const expansion& b) {
            return a.length() + b.length();
        }

        expansion& assign_sum(const expansion& a, const expansion& b);

        static index_t sum_capacity(
            const expansion& a, const expansion& b, const expansion& c
        ) {
            return a.length() + b.length() + c.length();
        }

        expansion& assign_sum(
            const expansion& a, const expansion& b, const expansion& c
        );

        static index_t sum_capacity(
            const expansion& a, const expansion& b,
            const expansion& c, const expansion& d
        ) {
            return a.length() + b.length() + c.length() + d.length();
        }

        expansion& assign_sum(
            const expansion& a, const expansion& b,
            const expansion& c, const expansion& d
        );

        static index_t diff_capacity(const expansion& a, const expansion& b) {
            return a.length() + b.length();
        }

        expansion& assign_diff(const expansion& a, const expansion& b);

        static index_t product_capacity(
            const expansion& a, const expansion& b
        ) {
            return a.length() * b.length() * 2;
        }

        expansion& assign_product(const expansion& a, const expansion& b);

        static index_t product_capacity(
            const expansion& a, const expansion& b, const expansion& c
        ) {
            return a.length() * b.length() * c.length() * 4;
        }

        expansion& assign_product(
            const expansion& a, const expansion& b, const expansion& c
        );

        static index_t square_capacity(const expansion& a) {
            if(a.length() == 2) {
                return 6;
            }                                  // see two_square()
            return a.length() * a.length() * 2;
        }

        expansion& assign_square(const expansion& a);

        // ====== Determinants =============================

        static index_t det2x2_capacity(
            const expansion& a11, const expansion& a12,
            const expansion& a21, const expansion& a22
        ) {
            return
                product_capacity(a11, a22) +
                product_capacity(a21, a12);
        }

        expansion& assign_det2x2(
            const expansion& a11, const expansion& a12,
            const expansion& a21, const expansion& a22
        );

        static index_t det3x3_capacity(
            const expansion& a11, const expansion& a12, const expansion& a13,
            const expansion& a21, const expansion& a22, const expansion& a23,
            const expansion& a31, const expansion& a32, const expansion& a33
        ) {
            // Development w.r.t. first row
            index_t c11_capa = det2x2_capacity(a22, a23, a32, a33);
            index_t c12_capa = det2x2_capacity(a21, a23, a31, a33);
            index_t c13_capa = det2x2_capacity(a21, a22, a31, a32);
            return 2 * (
                a11.length() * c11_capa +
                a12.length() * c12_capa +
                a13.length() * c13_capa
            );
        }

        expansion& assign_det3x3(
            const expansion& a11, const expansion& a12, const expansion& a13,
            const expansion& a21, const expansion& a22, const expansion& a23,
            const expansion& a31, const expansion& a32, const expansion& a33
        );

        static index_t det_111_2x3_capacity(
            const expansion& a21, const expansion& a22, const expansion& a23,
            const expansion& a31, const expansion& a32, const expansion& a33
        ) {
            return
                det2x2_capacity(a22, a23, a32, a33) +
                det2x2_capacity(a23, a21, a33, a31) +
                det2x2_capacity(a21, a22, a31, a32);
        }

        expansion& assign_det_111_2x3(
            const expansion& a21, const expansion& a22, const expansion& a23,
            const expansion& a31, const expansion& a32, const expansion& a33
        );

        // ======= Geometry-specific initializations =======

        static index_t sq_dist_capacity(coord_index_t dim) {
            return index_t(dim) * 6;
        }

        expansion& assign_sq_dist(
            const double* p1, const double* p2, coord_index_t dim
        );

        static index_t dot_at_capacity(coord_index_t dim) {
            return index_t(dim) * 8;
        }

        expansion& assign_dot_at(
            const double* p1, const double* p2, const double* p0,
            coord_index_t dim
        );


        static index_t length2_capacity(
            const expansion& x, const expansion& y, const expansion& z
        ) {
            return square_capacity(x) + square_capacity(y) + square_capacity(z);
        }

        expansion& assign_length2(
            const expansion& x, const expansion& y, const expansion& z
        );
        
        // =============== some general purpose functions =========

        static void initialize();

        expansion& negate() {
            for(index_t i = 0; i < length_; ++i) {
                x_[i] = -x_[i];
            }
            return *this;
        }

        expansion& scale_fast(double s) {
            // TODO: debug assert is_power_of_two(s)
            for(index_t i = 0; i < length_; ++i) {
                x_[i] *= s;
            }
            return *this;
        }

        double estimate() const {
            double result = 0.0;
            for(index_t i = 0; i < length(); ++i) {
                result += x_[i];
            }
            return result;
        }

        Sign sign() const {
            if(length() == 0) {
                return ZERO;
            }
            return geo_sgn(x_[length() - 1]);
        }

        std::ostream& show(std::ostream& os) const {
            for(index_t i = 0; i < length(); ++i) {
                os << i << ':' << x_[i] << ' ';
            }
            return os << std::endl;
        }

    protected:
        static index_t sub_product_capacity(
            index_t a_length, index_t b_length
        ) {
            return a_length * b_length * 2;
        }

        expansion& assign_sub_product(
            const double* a, index_t a_length, const expansion& b
        );

#define expansion_sub_product(a, a_length, b)           \
    new_expansion_on_stack(                       \
        sub_product_capacity(a_length, b.length()) \
    )->assign_sub_product(a, a_length, b)

    private:
        expansion(const expansion& rhs);

        expansion& operator= (const expansion& rhs);

    private:
        index_t length_;
        index_t capacity_;
        double x_[2];  // x_ is in fact of size [capacity_]

        friend class expansion_nt;
    };

    // =============== arithmetic operations ===========================

#define expansion_create(a)	      \
    new_expansion_on_stack(1)->assign(a)

    
#define expansion_sum(a, b)            \
    new_expansion_on_stack(           \
        expansion::sum_capacity(a, b)   \
    )->assign_sum(a, b)

#define expansion_sum3(a, b, c)          \
    new_expansion_on_stack(            \
        expansion::sum_capacity(a, b, c) \
    )->assign_sum(a, b, c)


#define expansion_sum4(a, b, c, d)          \
    new_expansion_on_stack(              \
        expansion::sum_capacity(a, b, c, d) \
    )->assign_sum(a, b, c, d)

#define expansion_diff(a, b)             \
    new_expansion_on_stack(             \
        expansion::diff_capacity(a, b)   \
    )->assign_diff(a, b)

#define expansion_product(a, b)            \
    new_expansion_on_stack(               \
        expansion::product_capacity(a, b)  \
    )->assign_product(a, b)

#define expansion_product3(a, b, c)           \
    new_expansion_on_stack(                 \
        expansion::product_capacity(a, b, c)  \
    )->assign_product(a, b, c)

#define expansion_square(a)             \
    new_expansion_on_stack(             \
        expansion::square_capacity(a)   \
    )->assign_square(a)

    // =============== determinants =====================================

#define expansion_det2x2(a11, a12, a21, a22)          \
    new_expansion_on_stack(                        \
        expansion::det2x2_capacity(a11, a12, a21, a22) \
    )->assign_det2x2(a11, a12, a21, a22)

#define expansion_det3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33)   \
    new_expansion_on_stack(                                             \
        expansion::det3x3_capacity(a11,a12,a13,a21,a22,a23,a31,a32,a33) \
    )->assign_det3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33)

#define expansion_det_111_2x3(a21, a22, a23, a31, a32, a33)           \
    new_expansion_on_stack(                                      \
        expansion::det_111_2x3_capacity(a21, a22, a23, a31, a32, a33) \
    )->assign_det_111_2x3(a21, a22, a23, a31, a32, a33)

    // =============== geometric functions ==============================

#define expansion_sq_dist(a, b, dim)           \
    new_expansion_on_stack(                  \
        expansion::sq_dist_capacity(dim)     \
    )->assign_sq_dist(a, b, dim)

#define expansion_dot_at(a, b, c, dim)           \
    new_expansion_on_stack(                   \
        expansion::dot_at_capacity(dim)       \
    )->assign_dot_at(a, b, c, dim)


#define expansion_length2(x,y,z)              \
    new_expansion_on_stack(                   \
       expansion::length2_capacity(x,y,z)     \
    )->assign_length2(x,y,z)
    
    

    Sign GEOGRAM_API sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,  
        const expansion& a10,const expansion& a11
    );
    
    Sign GEOGRAM_API sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,const expansion& a02,
        const expansion& a10,const expansion& a11,const expansion& a12,
        const expansion& a20,const expansion& a21,const expansion& a22
    );

    Sign GEOGRAM_API sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,
        const expansion& a02,const expansion& a03,
        const expansion& a10,const expansion& a11,
        const expansion& a12,const expansion& a13,
        const expansion& a20,const expansion& a21,
        const expansion& a22,const expansion& a23,
        const expansion& a30,const expansion& a31,
        const expansion& a32,const expansion& a33 
    );
    
    
}

#endif


/******* extracted from ../numerics/multi_precision.cpp *******/


// This makes sure the compiler will not optimize y = a*x+b
// with fused multiply-add, this would break the exact
// predicates.
#ifdef GEO_COMPILER_MSVC
#pragma fp_contract(off)
#endif


namespace {

    using namespace GEO;

    
    
    bool expansion_length_stat_ = false;
    std::vector<index_t> expansion_length_histo_;

    class ExpansionStatsDisplay {
    public:
        ~ExpansionStatsDisplay() {
            for(index_t i = 0; i < expansion_length_histo_.size(); ++i) {
                std::cerr << "expansion len " << i
                    << " : " << expansion_length_histo_[i] << std::endl;
            }
        }
    };

    ExpansionStatsDisplay expansion_stats_display_;

    

    class Pools {
    public:

        Pools() : pools_(1024,nullptr) {
            chunks_.reserve(1024);
        }

        ~Pools() {
            for(index_t i=0; i<chunks_.size(); ++i) {
                delete[] chunks_[i];
            }
        }

        void* malloc(size_t size) {
            if(size >= pools_.size()) {
                return ::malloc(size);
            }
            if(pools_[size] == nullptr) {
                new_chunk(size);
            }
            void* result = pools_[size];
            pools_[size] = *static_cast<void**>(pools_[size]);
            return result;
        }

        void free(void* ptr, size_t size) {
            if(size >= pools_.size()) {
                ::free(ptr);
                return;
            }
            *static_cast<void**>(ptr) = pools_[size];
            pools_[size] = ptr;
        }

        
    protected:
        static const index_t POOL_CHUNK_SIZE = 512;
        
        void new_chunk(size_t size_in) {
            size_t size = (size_in / 8 + 1)*8; // Align memory.
            Memory::pointer chunk = new Memory::byte[size * POOL_CHUNK_SIZE];
            for(index_t i=0; i<POOL_CHUNK_SIZE-1; ++i) {
                Memory::pointer cur = chunk + size * i;
                Memory::pointer next = cur + size;
                *reinterpret_cast<void**>(cur) = next;
            }
            *reinterpret_cast<void**>(chunk + (size-1)*POOL_CHUNK_SIZE) =
		pools_[size_in];
            pools_[size_in] = chunk;
            chunks_.push_back(chunk);
        }

        
    private:
        std::vector<void*> pools_;
        
        std::vector<Memory::pointer> chunks_;

    };

    static Pools pools_;
    
    
    
    inline void fast_two_sum(double a, double b, double& x, double& y) {
        x = a + b;
        double bvirt = x - a;
        y = b - bvirt;
    }

#ifdef REMOVE_ME        
    inline void fast_two_diff(double a, double b, double& x, double& y) {
        x = a - b;
        double bvirt = a - x;
        y = bvirt - b;
    }
#endif
    
    inline void two_one_sum(
        double a1, double a0, double b, double& x2, double& x1, double& x0
    ) {
        double _i;
        two_sum(a0, b, _i, x0);
        two_sum(a1, _i, x2, x1);
    }

    inline void two_two_sum(
        double a1, double a0, double b1, double b0,
        double& x3, double& x2, double& x1, double& x0
    ) {
        double _j, _0;
        two_one_sum(a1, a0, b0, _j, _0, x0);
        two_one_sum(_j, _0, b1, x3, x2, x1);
    }

    inline void two_product_presplit(
        double a, double b, double bhi, double blo, double& x, double& y
    ) {
        x = a * b;
        double ahi;
        double alo;
        split(a, ahi, alo);
        double err1 = x - (ahi * bhi);
        double err2 = err1 - (alo * bhi);
        double err3 = err2 - (ahi * blo);
        y = (alo * blo) - err3;
    }

    inline void two_product_2presplit(
        double a, double ahi, double alo,
        double b, double bhi, double blo,
        double& x, double& y
    ) {
        x = a * b;
        double err1 = x - (ahi * bhi);
        double err2 = err1 - (alo * bhi);
        double err3 = err2 - (ahi * blo);
        y = (alo * blo) - err3;
    }

    inline void two_square(
        double a1, double a0,
        double* x
    ) {
        double _0, _1, _2;
        double _j, _k, _l;
        square(a0, _j, x[0]);
        _0 = a0 + a0;
        two_product(a1, _0, _k, _1);
        two_one_sum(_k, _1, _j, _l, _2, x[1]);
        square(a1, _j, _1);
        two_two_sum(_j, _1, _l, _2, x[5], x[4], x[3], x[2]);
    }

    void two_two_product(
        const double* a,
        const double* b,
        double* x
    ) {
        double _0, _1, _2;
        double _i, _j, _k, _l, _m, _n;

        // If the target processor supports the FMA (Fused Multiply Add)
        // instruction, then the product of two doubles into a length-2
        // expansion can be implemented as follows. Thanks to Marc Glisse
        // for the information.
        // Note: under gcc, automatic generations of fma() for a*b+c needs
        // to be deactivated, using -ffp-contract=off, else it may break
        // other functions such as fast_expansion_sum_zeroelim().
#ifdef FP_FAST_FMA
        two_product(a[0],b[0],_i,x[0]);
        two_product(a[1],b[0],_j,_0);
        two_sum(_i, _0, _k, _1);
        fast_two_sum(_j, _k, _l, _2);
        two_product(a[0], b[1], _i, _0);
        two_sum(_1, _0, _k, x[1]);
        two_sum(_2, _k, _j, _1);
        two_sum(_l, _j, _m, _2);
        two_product(a[1], b[1], _j, _0);
        two_sum(_i, _0, _n, _0);
        two_sum(_1, _0, _i, x[2]);
        two_sum(_2, _i, _k, _1);
        two_sum(_m, _k, _l, _2);
        two_sum(_j, _n, _k, _0);
        two_sum(_1, _0, _j, x[3]);
        two_sum(_2, _j, _i, _1);
        two_sum(_l, _i, _m, _2);
        two_sum(_1, _k, _i, x[4]);
        two_sum(_2, _i, _k, x[5]);
        two_sum(_m, _k, x[7], x[6]);
#else
        double a0hi, a0lo;
        split(a[0], a0hi, a0lo);
        double bhi, blo;
        split(b[0], bhi, blo);
        two_product_2presplit(
            a[0], a0hi, a0lo, b[0], bhi, blo, _i, x[0]
        );
        double a1hi, a1lo;
        split(a[1], a1hi, a1lo);
        two_product_2presplit(
            a[1], a1hi, a1lo, b[0], bhi, blo, _j, _0
        );
        two_sum(_i, _0, _k, _1);
        fast_two_sum(_j, _k, _l, _2);
        split(b[1], bhi, blo);
        two_product_2presplit(
            a[0], a0hi, a0lo, b[1], bhi, blo, _i, _0
        );
        two_sum(_1, _0, _k, x[1]);
        two_sum(_2, _k, _j, _1);
        two_sum(_l, _j, _m, _2);
        two_product_2presplit(
            a[1], a1hi, a1lo, b[1], bhi, blo, _j, _0
        );
        two_sum(_i, _0, _n, _0);
        two_sum(_1, _0, _i, x[2]);
        two_sum(_2, _i, _k, _1);
        two_sum(_m, _k, _l, _2);
        two_sum(_j, _n, _k, _0);
        two_sum(_1, _0, _j, x[3]);
        two_sum(_2, _j, _i, _1);
        two_sum(_l, _i, _m, _2);
        two_sum(_1, _k, _i, x[4]);
        two_sum(_2, _i, _k, x[5]);
        two_sum(_m, _k, x[7], x[6]);
#endif
    }

    void grow_expansion_zeroelim(
        const expansion& e, double b, expansion& h
    ) {
        double Q, hh;
        double Qnew;
        index_t eindex, hindex;
        index_t elen = e.length();

        hindex = 0;
        Q = b;
        for(eindex = 0; eindex < elen; eindex++) {
            double enow = e[eindex];
            two_sum(Q, enow, Qnew, hh);
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
        }
        if((Q != 0.0) || (hindex == 0)) {
            h[hindex++] = Q;
        }
        h.set_length(hindex);
    }

    void scale_expansion_zeroelim(
        const expansion& e, double b, expansion& h
    ) {
        double Q, sum;
        double hh;
        double product1;
        double product0;
        index_t eindex, hindex;

        // If the target processor supports the FMA (Fused Multiply Add)
        // instruction, then the product of two doubles into a length-2
        // expansion can be implemented as follows. Thanks to Marc Glisse
        // for the information.
        // Note: under gcc, automatic generations of fma() for a*b+c needs
        // to be deactivated, using -ffp-contract=off, else it may break
        // other functions such as fast_expansion_sum_zeroelim().
#ifndef FP_FAST_FMA
        double bhi, blo;
#endif
        index_t elen = e.length();

        // Sanity check: e and h cannot be the same.
        geo_debug_assert(&e != &h);

#ifdef FP_FAST_FMA
        two_product(e[0], b, Q, hh);
#else
        split(b, bhi, blo);
        two_product_presplit(e[0], b, bhi, blo, Q, hh);
#endif

        hindex = 0;
        if(hh != 0) {
            h[hindex++] = hh;
        }
        for(eindex = 1; eindex < elen; eindex++) {
            double enow = e[eindex];
#ifdef FP_FAST_FMA
            two_product(enow, b,  product1, product0);
#else
            two_product_presplit(enow, b, bhi, blo, product1, product0);
#endif
            two_sum(Q, product0, sum, hh);
            if(hh != 0) {
                h[hindex++] = hh;
            }
            fast_two_sum(product1, sum, Q, hh);
            if(hh != 0) {
                h[hindex++] = hh;
            }
        }
        if((Q != 0.0) || (hindex == 0)) {
            h[hindex++] = Q;
        }
        h.set_length(hindex);
    }

    void fast_expansion_sum_zeroelim(
        const expansion& e, const expansion& f, expansion& h
    ) {
        double Q;
        double Qnew;
        double hh;
        index_t eindex, findex, hindex;
        double enow, fnow;
        index_t elen = e.length();
        index_t flen = f.length();

        // sanity check: h cannot be e or f
        geo_debug_assert(&h != &e);
        geo_debug_assert(&h != &f);

        enow = e[0];
        fnow = f[0];
        eindex = findex = 0;
        if((fnow > enow) == (fnow > -enow)) {
            Q = enow;
            enow = e[++eindex];
        } else {
            Q = fnow;
            fnow = f[++findex];
        }
        hindex = 0;
        if((eindex < elen) && (findex < flen)) {
            if((fnow > enow) == (fnow > -enow)) {
                fast_two_sum(enow, Q, Qnew, hh);
                enow = e[++eindex];
            } else {
                fast_two_sum(fnow, Q, Qnew, hh);
                fnow = f[++findex];
            }
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
            while((eindex < elen) && (findex < flen)) {
                if((fnow > enow) == (fnow > -enow)) {
                    two_sum(Q, enow, Qnew, hh);
                    enow = e[++eindex];
                } else {
                    two_sum(Q, fnow, Qnew, hh);
                    fnow = f[++findex];
                }
                Q = Qnew;
                if(hh != 0.0) {
                    h[hindex++] = hh;
                }
            }
        }
        while(eindex < elen) {
            two_sum(Q, enow, Qnew, hh);
            enow = e[++eindex];
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
        }
        while(findex < flen) {
            two_sum(Q, fnow, Qnew, hh);
            fnow = f[++findex];
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
        }
        if((Q != 0.0) || (hindex == 0)) {
            h[hindex++] = Q;
        }
        h.set_length(hindex);
    }

    void fast_expansion_diff_zeroelim(
        const expansion& e, const expansion& f, expansion& h
    ) {
        double Q;
        double Qnew;
        double hh;
        index_t eindex, findex, hindex;
        double enow, fnow;
        index_t elen = e.length();
        index_t flen = f.length();

        // sanity check: h cannot be e or f
        geo_debug_assert(&h != &e);
        geo_debug_assert(&h != &f);

        enow = e[0];
        fnow = -f[0];
        eindex = findex = 0;
        if((fnow > enow) == (fnow > -enow)) {
            Q = enow;
            enow = e[++eindex];
        } else {
            Q = fnow;
            fnow = -f[++findex];
        }
        hindex = 0;
        if((eindex < elen) && (findex < flen)) {
            if((fnow > enow) == (fnow > -enow)) {
                fast_two_sum(enow, Q, Qnew, hh);
                enow = e[++eindex];
            } else {
                fast_two_sum(fnow, Q, Qnew, hh);
                fnow = -f[++findex];
            }
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
            while((eindex < elen) && (findex < flen)) {
                if((fnow > enow) == (fnow > -enow)) {
                    two_sum(Q, enow, Qnew, hh);
                    enow = e[++eindex];
                } else {
                    two_sum(Q, fnow, Qnew, hh);
                    fnow = -f[++findex];
                }
                Q = Qnew;
                if(hh != 0.0) {
                    h[hindex++] = hh;
                }
            }
        }
        while(eindex < elen) {
            two_sum(Q, enow, Qnew, hh);
            enow = e[++eindex];
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
        }
        while(findex < flen) {
            two_sum(Q, fnow, Qnew, hh);
            fnow = -f[++findex];
            Q = Qnew;
            if(hh != 0.0) {
                h[hindex++] = hh;
            }
        }
        if((Q != 0.0) || (hindex == 0)) {
            h[hindex++] = Q;
        }
        h.set_length(hindex);
    }
}



namespace GEO {

    double expansion_splitter_;
    double expansion_epsilon_;

    void expansion::initialize() {
        // Taken from Jonathan Shewchuk's exactinit.
        double half;
        double check, lastcheck;
        int every_other;

        every_other = 1;
        half = 0.5;
        expansion_epsilon_ = 1.0;
        expansion_splitter_ = 1.0;
        check = 1.0;
        // Repeatedly divide `epsilon' by two until it is too small to add to
        // one without causing roundoff.  (Also check if the sum is equal to
        // the previous sum, for machines that round up instead of using exact
        // rounding.  Not that this library will work on such machines anyway.
        do {
            lastcheck = check;
            expansion_epsilon_ *= half;
            if(every_other) {
                expansion_splitter_ *= 2.0;
            }
            every_other = !every_other;
            check = 1.0 + expansion_epsilon_;
        } while((check != 1.0) && (check != lastcheck));
        expansion_splitter_ += 1.0;
    }

    static Process::spinlock expansions_lock = GEOGRAM_SPINLOCK_INIT;
    
    expansion* expansion::new_expansion_on_heap(index_t capa) {
	Process::acquire_spinlock(expansions_lock);
        if(expansion_length_stat_) {
            if(capa >= expansion_length_histo_.size()) {
                expansion_length_histo_.resize(capa + 1);
            }
            expansion_length_histo_[capa]++;
        }
        Memory::pointer addr = Memory::pointer(
            pools_.malloc(expansion::bytes(capa))
        );
	Process::release_spinlock(expansions_lock);
        expansion* result = new(addr)expansion(capa);
        return result;
    }

    void expansion::delete_expansion_on_heap(expansion* e) {
	Process::acquire_spinlock(expansions_lock);	
        pools_.free(e, expansion::bytes(e->capacity()));
	Process::release_spinlock(expansions_lock);	
    }

    // ====== Initialization from expansion and double ===============

    expansion& expansion::assign_sum(const expansion& a, double b) {
        geo_debug_assert(capacity() >= sum_capacity(a, b));
        grow_expansion_zeroelim(a, b, *this);
        return *this;
    }

    expansion& expansion::assign_diff(const expansion& a, double b) {
        geo_debug_assert(capacity() >= diff_capacity(a, b));
        grow_expansion_zeroelim(a, -b, *this);
        return *this;
    }

    expansion& expansion::assign_product(const expansion& a, double b) {
        // TODO: implement special case where the double argument
        // is a power of two.
        geo_debug_assert(capacity() >= product_capacity(a, b));
        scale_expansion_zeroelim(a, b, *this);
        return *this;
    }

    // =============  expansion sum and difference =========================

    expansion& expansion::assign_sum(
        const expansion& a, const expansion& b
    ) {
        geo_debug_assert(capacity() >= sum_capacity(a, b));
        fast_expansion_sum_zeroelim(a, b, *this);
        return *this;
    }

    expansion& expansion::assign_sum(
        const expansion& a, const expansion& b, const expansion& c
    ) {
        geo_debug_assert(capacity() >= sum_capacity(a, b, c));
        expansion& ab = expansion_sum(a, b);
        this->assign_sum(ab, c);
        return *this;
    }

    expansion& expansion::assign_sum(
        const expansion& a, const expansion& b,
        const expansion& c, const expansion& d
    ) {
        geo_debug_assert(capacity() >= sum_capacity(a, b, c));
        expansion& ab = expansion_sum(a, b);
        expansion& cd = expansion_sum(c, d);
        this->assign_sum(ab, cd);
        return *this;
    }

    expansion& expansion::assign_diff(const expansion& a, const expansion& b) {
        geo_debug_assert(capacity() >= diff_capacity(a, b));
        fast_expansion_diff_zeroelim(a, b, *this);
        return *this;
    }

    // =============  expansion product ==================================

    // Recursive helper function for product implementation
    expansion& expansion::assign_sub_product(
        const double* a, index_t a_length, const expansion& b
    ) {
        geo_debug_assert(
            capacity() >= sub_product_capacity(a_length, b.length())
        );
        if(a_length == 1) {
            scale_expansion_zeroelim(b, a[0], *this);
        } else {
            // "Distillation" (see Shewchuk's paper) is computed recursively,
            // by splitting the list of expansions to sum into two halves.
            const double* a1 = a;
            index_t a1_length = a_length / 2;
            const double* a2 = a1 + a1_length;
            index_t a2_length = a_length - a1_length;
            expansion& a1b = expansion_sub_product(a1, a1_length, b);
            expansion& a2b = expansion_sub_product(a2, a2_length, b);
            this->assign_sum(a1b, a2b);
        }
        return *this;
    }

    expansion& expansion::assign_product(
        const expansion& a, const expansion& b
    ) {
        geo_debug_assert(capacity() >= product_capacity(a, b));
        if(a.length() == 0 || b.length() == 0) {
            x_[0] = 0.0;
            set_length(0);
        } else if(a.length() == 1 && b.length() == 1) {
            two_product(a[0], b[0], x_[1], x_[0]);
            set_length(2);
        } else if(a.length() == 1) {
            scale_expansion_zeroelim(b, a[0], *this);
        } else if(b.length() == 1) {
            scale_expansion_zeroelim(a, b[0], *this);
        } else if(a.length() == 2 && b.length() == 2) {
            two_two_product(a.data(), b.data(), x_);
            set_length(8);
        } else {
            // Recursive distillation: the shortest expansion
            // is split into two parts.
            if(a.length() < b.length()) {
                const double* a1 = a.data();
                index_t a1_length = a.length() / 2;
                const double* a2 = a1 + a1_length;
                index_t a2_length = a.length() - a1_length;
                expansion& a1b = expansion_sub_product(a1, a1_length, b);
                expansion& a2b = expansion_sub_product(a2, a2_length, b);
                this->assign_sum(a1b, a2b);
            } else {
                const double* b1 = b.data();
                index_t b1_length = b.length() / 2;
                const double* b2 = b1 + b1_length;
                index_t b2_length = b.length() - b1_length;
                expansion& ab1 = expansion_sub_product(b1, b1_length, a);
                expansion& ab2 = expansion_sub_product(b2, b2_length, a);
                this->assign_sum(ab1, ab2);
            }
        }
        return *this;
    }

    expansion& expansion::assign_product(
        const expansion& a, const expansion& b, const expansion& c
    ) {
        const expansion& bc = expansion_product(b, c);
        this->assign_product(a, bc);
        return *this;
    }

    expansion& expansion::assign_square(const expansion& a) {
        geo_debug_assert(capacity() >= square_capacity(a));
        if(a.length() == 1) {
            square(a[0], x_[1], x_[0]);
            set_length(2);
        } else if(a.length() == 2) {
            two_square(a[1], a[0], x_);
            set_length(6);
        } else {
            this->assign_product(a, a);
        }
        return *this;
    }

    // =============  determinants ==========================================

    expansion& expansion::assign_det2x2(
        const expansion& a11, const expansion& a12,
        const expansion& a21, const expansion& a22
    ) {
        const expansion& a11a22 = expansion_product(a11, a22);
        const expansion& a12a21 = expansion_product(a12, a21);
        return this->assign_diff(a11a22, a12a21);
    }

    expansion& expansion::assign_det3x3(
        const expansion& a11, const expansion& a12, const expansion& a13,
        const expansion& a21, const expansion& a22, const expansion& a23,
        const expansion& a31, const expansion& a32, const expansion& a33
    ) {
        // Development w.r.t. first row
        const expansion& c11 = expansion_det2x2(a22, a23, a32, a33);
        const expansion& c12 = expansion_det2x2(a23, a21, a33, a31);
        const expansion& c13 = expansion_det2x2(a21, a22, a31, a32);
        const expansion& a11c11 = expansion_product(a11, c11);
        const expansion& a12c12 = expansion_product(a12, c12);
        const expansion& a13c13 = expansion_product(a13, c13);
        return this->assign_sum(a11c11, a12c12, a13c13);
    }

    expansion& expansion::assign_det_111_2x3(
        const expansion& a21, const expansion& a22, const expansion& a23,
        const expansion& a31, const expansion& a32, const expansion& a33
    ) {
        const expansion& c11 = expansion_det2x2(a22, a23, a32, a33);
        const expansion& c12 = expansion_det2x2(a23, a21, a33, a31);
        const expansion& c13 = expansion_det2x2(a21, a22, a31, a32);
        return this->assign_sum(c11, c12, c13);
    }

    // =============  geometric operations ==================================

    expansion& expansion::assign_sq_dist(
        const double* p1, const double* p2, coord_index_t dim
    ) {
        geo_debug_assert(capacity() >= sq_dist_capacity(dim));
	geo_debug_assert(dim > 0);
        if(dim == 1) {
            double d0, d1;
            two_diff(p1[0], p2[0], d1, d0);
            two_square(d1, d0, x_);
            set_length(6);
        } else {
            // "Distillation" (see Shewchuk's paper) is computed recursively,
            // by splitting the list of expansions to sum into two halves.
            coord_index_t dim1 = dim / 2;
            coord_index_t dim2 = coord_index_t(dim - dim1);
            const double* p1_2 = p1 + dim1;
            const double* p2_2 = p2 + dim1;
            expansion& d1 = expansion_sq_dist(p1, p2, dim1);
            expansion& d2 = expansion_sq_dist(p1_2, p2_2, dim2);
            this->assign_sum(d1, d2);
        }
        return *this;
    }

    expansion& expansion::assign_dot_at(
        const double* p1, const double* p2, const double* p0,
        coord_index_t dim
    ) {
        geo_debug_assert(capacity() >= dot_at_capacity(dim));
        if(dim == 1) {

            double v[2];
            two_diff(p1[0], p0[0], v[1], v[0]);
            double w[2];
            two_diff(p2[0], p0[0], w[1], w[0]);
            two_two_product(v, w, x_);
            set_length(8);
        } else {
            // "Distillation" (see Shewchuk's paper) is computed recursively,
            // by splitting the list of expansions to sum into two halves.
            coord_index_t dim1 = dim / 2;
            coord_index_t dim2 = coord_index_t(dim - dim1);
            const double* p1_2 = p1 + dim1;
            const double* p2_2 = p2 + dim1;
            const double* p0_2 = p0 + dim1;
            expansion& d1 = expansion_dot_at(p1, p2, p0, dim1);
            expansion& d2 = expansion_dot_at(p1_2, p2_2, p0_2, dim2);
            this->assign_sum(d1, d2);
        } 
        return *this;
    }

    expansion& expansion::assign_length2(
        const expansion& x, const expansion& y, const expansion& z
    ) {
        const expansion& x2 = expansion_square(x);
        const expansion& y2 = expansion_square(y);
        const expansion& z2 = expansion_square(z);
        this->assign_sum(x2,y2,z2);
        return *this;
    }
    
    

    Sign sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,  
        const expansion& a10,const expansion& a11
    ) {
        const expansion& result = expansion_det2x2(a00, a01, a10, a11);
        return result.sign();
    }

    Sign sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,const expansion& a02,
        const expansion& a10,const expansion& a11,const expansion& a12,
        const expansion& a20,const expansion& a21,const expansion& a22
    ) {
        // First compute the det2x2
        const expansion& m01 =
            expansion_det2x2(a00, a10, a01, a11); 
        const expansion& m02 =
            expansion_det2x2(a00, a20, a01, a21);
        const expansion& m12 =
            expansion_det2x2(a10, a20, a11, a21);

        // Now compute the minors of rank 3
        const expansion& z1 = expansion_product(m01,a22);
        const expansion& z2 = expansion_product(m02,a12).negate();
        const expansion& z3 = expansion_product(m12,a02);

        const expansion& result = expansion_sum3(z1,z2,z3);
        return result.sign();
    }
    
    Sign sign_of_expansion_determinant(
        const expansion& a00,const expansion& a01,
        const expansion& a02,const expansion& a03,
        const expansion& a10,const expansion& a11,
        const expansion& a12,const expansion& a13,
        const expansion& a20,const expansion& a21,
        const expansion& a22,const expansion& a23,
        const expansion& a30,const expansion& a31,
        const expansion& a32,const expansion& a33 
    ) {

        // First compute the det2x2        
        const expansion& m01 =
            expansion_det2x2(a10,a00,a11,a01);
        const expansion& m02 =
            expansion_det2x2(a20,a00,a21,a01);
        const expansion& m03 =
            expansion_det2x2(a30,a00,a31,a01);
        const expansion& m12 =
            expansion_det2x2(a20,a10,a21,a11);
        const expansion& m13 =
            expansion_det2x2(a30,a10,a31,a11);
        const expansion& m23 =
            expansion_det2x2(a30,a20,a31,a21);     
        
        // Now compute the minors of rank 3
        const expansion& m012_1 = expansion_product(m12,a02);
        expansion& m012_2 = expansion_product(m02,a12); m012_2.negate();
        const expansion& m012_3 = expansion_product(m01,a22);
        const expansion& m012 = expansion_sum3(m012_1, m012_2, m012_3);

        const expansion& m013_1 = expansion_product(m13,a02);
        expansion& m013_2 = expansion_product(m03,a12); m013_2.negate();
        
        const expansion& m013_3 = expansion_product(m01,a32);
        const expansion& m013 = expansion_sum3(m013_1, m013_2, m013_3);
        
        const expansion& m023_1 = expansion_product(m23,a02);
        expansion& m023_2 = expansion_product(m03,a22); m023_2.negate();
        const expansion& m023_3 = expansion_product(m02,a32);
        const expansion& m023 = expansion_sum3(m023_1, m023_2, m023_3);

        const expansion& m123_1 = expansion_product(m23,a12);
        expansion& m123_2 = expansion_product(m13,a22); m123_2.negate();
        const expansion& m123_3 = expansion_product(m12,a32);
        const expansion& m123 = expansion_sum3(m123_1, m123_2, m123_3);
        
        // Now compute the minors of rank 4
        const expansion& m0123_1 = expansion_product(m123,a03);
        const expansion& m0123_2 = expansion_product(m023,a13);
        const expansion& m0123_3 = expansion_product(m013,a23);
        const expansion& m0123_4 = expansion_product(m012,a33);

        const expansion& z1 = expansion_sum(m0123_1, m0123_3);
        const expansion& z2 = expansion_sum(m0123_2, m0123_4);

        const expansion& result = expansion_diff(z1,z2);
        return result.sign();
    }
    
    
    
}


/******* extracted from ../numerics/predicates/side1.h *******/

inline int side1_3d_filter( const double* p0, const double* p1, const double* q0) {
    double p0_0_p1_0 = (p0[0] - p1[0]);
    double p0_1_p1_1 = (p0[1] - p1[1]);
    double p0_2_p1_2 = (p0[2] - p1[2]);
    double r;
    r = (1 * (((p0_0_p1_0 * p0_0_p1_0) + (p0_1_p1_1 * p0_1_p1_1)) + (p0_2_p1_2 * p0_2_p1_2)));
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    r = (r - (2 * (((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0_0_p1_0);
    if( (max1 < fabs(p0_1_p1_1)) )
    {
        max1 = fabs(p0_1_p1_1);
    } 
    if( (max1 < fabs(p0_2_p1_2)) )
    {
        max1 = fabs(p0_2_p1_2);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    double max2 = fabs(p0_0_p1_0);
    if( (max2 < fabs(p0_1_p1_1)) )
    {
        max2 = fabs(p0_1_p1_1);
    } 
    if( (max2 < fabs(p0_2_p1_2)) )
    {
        max2 = fabs(p0_2_p1_2);
    } 
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 2.23755023300058943229e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.44425370757048798480e-15 * (max1 * max2));
        if( (r > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


inline int side1_4d_filter( const double* p0, const double* p1, const double* q0) {
    double p0_0_p1_0 = (p0[0] - p1[0]);
    double p0_1_p1_1 = (p0[1] - p1[1]);
    double p0_2_p1_2 = (p0[2] - p1[2]);
    double p0_3_p1_3 = (p0[3] - p1[3]);
    double r;
    r = (1 * ((((p0_0_p1_0 * p0_0_p1_0) + (p0_1_p1_1 * p0_1_p1_1)) + (p0_2_p1_2 * p0_2_p1_2)) + (p0_3_p1_3 * p0_3_p1_3)));
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    r = (r - (2 * ((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p1_0_p0_0);
    if( (max1 < fabs(p0_0_p1_0)) )
    {
        max1 = fabs(p0_0_p1_0);
    } 
    if( (max1 < fabs(p0_1_p1_1)) )
    {
        max1 = fabs(p0_1_p1_1);
    } 
    if( (max1 < fabs(p0_2_p1_2)) )
    {
        max1 = fabs(p0_2_p1_2);
    } 
    if( (max1 < fabs(p0_3_p1_3)) )
    {
        max1 = fabs(p0_3_p1_3);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    double max2 = fabs(p0_0_p1_0);
    if( (max2 < fabs(p0_1_p1_1)) )
    {
        max2 = fabs(p0_1_p1_1);
    } 
    if( (max2 < fabs(p0_2_p1_2)) )
    {
        max2 = fabs(p0_2_p1_2);
    } 
    if( (max2 < fabs(p0_3_p1_3)) )
    {
        max2 = fabs(p0_3_p1_3);
    } 
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (lower_bound_1 < 1.85816790703293534018e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (6.44428177279185717888e-15 * (max1 * max2));
        if( (r > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


inline int side1_6d_filter( const double* p0, const double* p1, const double* q0) {
    double p0_0_p1_0 = (p0[0] - p1[0]);
    double p0_1_p1_1 = (p0[1] - p1[1]);
    double p0_2_p1_2 = (p0[2] - p1[2]);
    double p0_3_p1_3 = (p0[3] - p1[3]);
    double p0_4_p1_4 = (p0[4] - p1[4]);
    double p0_5_p1_5 = (p0[5] - p1[5]);
    double r;
    r = (1 * ((((((p0_0_p1_0 * p0_0_p1_0) + (p0_1_p1_1 * p0_1_p1_1)) + (p0_2_p1_2 * p0_2_p1_2)) + (p0_3_p1_3 * p0_3_p1_3)) + (p0_4_p1_4 * p0_4_p1_4)) + (p0_5_p1_5 * p0_5_p1_5)));
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    r = (r - (2 * ((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0_0_p1_0);
    if( (max1 < fabs(p0_1_p1_1)) )
    {
        max1 = fabs(p0_1_p1_1);
    } 
    if( (max1 < fabs(p0_2_p1_2)) )
    {
        max1 = fabs(p0_2_p1_2);
    } 
    if( (max1 < fabs(p0_3_p1_3)) )
    {
        max1 = fabs(p0_3_p1_3);
    } 
    if( (max1 < fabs(p0_4_p1_4)) )
    {
        max1 = fabs(p0_4_p1_4);
    } 
    if( (max1 < fabs(p0_5_p1_5)) )
    {
        max1 = fabs(p0_5_p1_5);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    double max2 = fabs(p0_0_p1_0);
    if( (max2 < fabs(p0_1_p1_1)) )
    {
        max2 = fabs(p0_1_p1_1);
    } 
    if( (max2 < fabs(p0_2_p1_2)) )
    {
        max2 = fabs(p0_2_p1_2);
    } 
    if( (max2 < fabs(p0_3_p1_3)) )
    {
        max2 = fabs(p0_3_p1_3);
    } 
    if( (max2 < fabs(p0_4_p1_4)) )
    {
        max2 = fabs(p0_4_p1_4);
    } 
    if( (max2 < fabs(p0_5_p1_5)) )
    {
        max2 = fabs(p0_5_p1_5);
    } 
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.41511993781011659868e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.11111223981318615596e-14 * (max1 * max2));
        if( (r > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


inline int side1_7d_filter( const double* p0, const double* p1, const double* q0) {
    double p0_0_p1_0 = (p0[0] - p1[0]);
    double p0_1_p1_1 = (p0[1] - p1[1]);
    double p0_2_p1_2 = (p0[2] - p1[2]);
    double p0_3_p1_3 = (p0[3] - p1[3]);
    double p0_4_p1_4 = (p0[4] - p1[4]);
    double p0_5_p1_5 = (p0[5] - p1[5]);
    double p0_6_p1_6 = (p0[6] - p1[6]);
    double r;
    r = (1 * (((((((p0_0_p1_0 * p0_0_p1_0) + (p0_1_p1_1 * p0_1_p1_1)) + (p0_2_p1_2 * p0_2_p1_2)) + (p0_3_p1_3 * p0_3_p1_3)) + (p0_4_p1_4 * p0_4_p1_4)) + (p0_5_p1_5 * p0_5_p1_5)) + (p0_6_p1_6 * p0_6_p1_6)));
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    r = (r - (2 * (((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0_0_p1_0);
    if( (max1 < fabs(p0_1_p1_1)) )
    {
        max1 = fabs(p0_1_p1_1);
    } 
    if( (max1 < fabs(p0_2_p1_2)) )
    {
        max1 = fabs(p0_2_p1_2);
    } 
    if( (max1 < fabs(p0_3_p1_3)) )
    {
        max1 = fabs(p0_3_p1_3);
    } 
    if( (max1 < fabs(p0_4_p1_4)) )
    {
        max1 = fabs(p0_4_p1_4);
    } 
    if( (max1 < fabs(p0_5_p1_5)) )
    {
        max1 = fabs(p0_5_p1_5);
    } 
    if( (max1 < fabs(p0_6_p1_6)) )
    {
        max1 = fabs(p0_6_p1_6);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    double max2 = fabs(p0_0_p1_0);
    if( (max2 < fabs(p0_1_p1_1)) )
    {
        max2 = fabs(p0_1_p1_1);
    } 
    if( (max2 < fabs(p0_2_p1_2)) )
    {
        max2 = fabs(p0_2_p1_2);
    } 
    if( (max2 < fabs(p0_3_p1_3)) )
    {
        max2 = fabs(p0_3_p1_3);
    } 
    if( (max2 < fabs(p0_4_p1_4)) )
    {
        max2 = fabs(p0_4_p1_4);
    } 
    if( (max2 < fabs(p0_5_p1_5)) )
    {
        max2 = fabs(p0_5_p1_5);
    } 
    if( (max2 < fabs(p0_6_p1_6)) )
    {
        max2 = fabs(p0_6_p1_6);
    } 
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    if( (max2 < fabs(q0_6_p0_6)) )
    {
        max2 = fabs(q0_6_p0_6);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.27080861580266953580e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.37779349582504943796e-14 * (max1 * max2));
        if( (r > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


inline int side1_8d_filter( const double* p0, const double* p1, const double* q0) {
    double p0_0_p1_0 = (p0[0] - p1[0]);
    double p0_1_p1_1 = (p0[1] - p1[1]);
    double p0_2_p1_2 = (p0[2] - p1[2]);
    double p0_3_p1_3 = (p0[3] - p1[3]);
    double p0_4_p1_4 = (p0[4] - p1[4]);
    double p0_5_p1_5 = (p0[5] - p1[5]);
    double p0_6_p1_6 = (p0[6] - p1[6]);
    double p0_7_p1_7 = (p0[7] - p1[7]);
    double r;
    r = (1 * ((((((((p0_0_p1_0 * p0_0_p1_0) + (p0_1_p1_1 * p0_1_p1_1)) + (p0_2_p1_2 * p0_2_p1_2)) + (p0_3_p1_3 * p0_3_p1_3)) + (p0_4_p1_4 * p0_4_p1_4)) + (p0_5_p1_5 * p0_5_p1_5)) + (p0_6_p1_6 * p0_6_p1_6)) + (p0_7_p1_7 * p0_7_p1_7)));
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double p1_7_p0_7 = (p1[7] - p0[7]);
    double q0_7_p0_7 = (q0[7] - p0[7]);
    r = (r - (2 * ((((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)) + (p1_7_p0_7 * q0_7_p0_7))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0_1_p1_1);
    if( (max1 < fabs(p0_2_p1_2)) )
    {
        max1 = fabs(p0_2_p1_2);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p0_0_p1_0)) )
    {
        max1 = fabs(p0_0_p1_0);
    } 
    if( (max1 < fabs(p0_3_p1_3)) )
    {
        max1 = fabs(p0_3_p1_3);
    } 
    if( (max1 < fabs(p0_4_p1_4)) )
    {
        max1 = fabs(p0_4_p1_4);
    } 
    if( (max1 < fabs(p0_5_p1_5)) )
    {
        max1 = fabs(p0_5_p1_5);
    } 
    if( (max1 < fabs(p0_6_p1_6)) )
    {
        max1 = fabs(p0_6_p1_6);
    } 
    if( (max1 < fabs(p0_7_p1_7)) )
    {
        max1 = fabs(p0_7_p1_7);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    if( (max1 < fabs(p1_7_p0_7)) )
    {
        max1 = fabs(p1_7_p0_7);
    } 
    double max2 = fabs(p0_1_p1_1);
    if( (max2 < fabs(p0_2_p1_2)) )
    {
        max2 = fabs(p0_2_p1_2);
    } 
    if( (max2 < fabs(p0_0_p1_0)) )
    {
        max2 = fabs(p0_0_p1_0);
    } 
    if( (max2 < fabs(p0_3_p1_3)) )
    {
        max2 = fabs(p0_3_p1_3);
    } 
    if( (max2 < fabs(p0_4_p1_4)) )
    {
        max2 = fabs(p0_4_p1_4);
    } 
    if( (max2 < fabs(p0_5_p1_5)) )
    {
        max2 = fabs(p0_5_p1_5);
    } 
    if( (max2 < fabs(p0_6_p1_6)) )
    {
        max2 = fabs(p0_6_p1_6);
    } 
    if( (max2 < fabs(p0_7_p1_7)) )
    {
        max2 = fabs(p0_7_p1_7);
    } 
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    if( (max2 < fabs(q0_6_p0_6)) )
    {
        max2 = fabs(q0_6_p0_6);
    } 
    if( (max2 < fabs(q0_7_p0_7)) )
    {
        max2 = fabs(q0_7_p0_7);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.15542931091530087067e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.66670090166682227006e-14 * (max1 * max2));
        if( (r > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


/******* extracted from ../numerics/predicates/side2.h *******/

inline int side2_3d_filter( const double* p0, const double* p1, const double* p2, const double* q0, const double* q1) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double l1;
    l1 = (1 * (((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double l2;
    l2 = (1 * (((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double a10;
    a10 = (2 * (((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double a11;
    a11 = (2 * (((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)));
    double a20;
    a20 = (2 * (((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)));
    double a21;
    a21 = (2 * (((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)));
    double Delta;
    Delta = (a11 - a10);
    double DeltaLambda0;
    DeltaLambda0 = (a11 - l1);
    double DeltaLambda1;
    DeltaLambda1 = (l1 - a10);
    double r;
    r = (((Delta * l2) - (a20 * DeltaLambda0)) - (a21 * DeltaLambda1));
    double eps;
    double max1 = fabs(p1_0_p0_0);
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 2.23755023300058943229e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 3.74144419156711063983e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.44425370757048798480e-15 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max3 = max1;
    if( (max3 < max2) )
    {
        max3 = max2;
    } 
    double max4 = max2;
    if( (max4 < fabs(p2_0_p0_0)) )
    {
        max4 = fabs(p2_0_p0_0);
    } 
    if( (max4 < fabs(p2_1_p0_1)) )
    {
        max4 = fabs(p2_1_p0_1);
    } 
    if( (max4 < fabs(p2_2_p0_2)) )
    {
        max4 = fabs(p2_2_p0_2);
    } 
    if( (max3 < max4) )
    {
        max3 = max4;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    if( (lower_bound_1 < 2.22985945097100191780e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 3.74144419156711063983e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.99983341597279045654e-14 * (((max1 * max4) * max4) * max3));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side2_4d_filter( const double* p0, const double* p1, const double* p2, const double* q0, const double* q1) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double l1;
    l1 = (1 * ((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double l2;
    l2 = (1 * ((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double a10;
    a10 = (2 * ((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double a11;
    a11 = (2 * ((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)));
    double a20;
    a20 = (2 * ((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)));
    double a21;
    a21 = (2 * ((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)));
    double Delta;
    Delta = (a11 - a10);
    double DeltaLambda0;
    DeltaLambda0 = (a11 - l1);
    double DeltaLambda1;
    DeltaLambda1 = (l1 - a10);
    double r;
    r = (((Delta * l2) - (a20 * DeltaLambda0)) - (a21 * DeltaLambda1));
    double eps;
    double max1 = fabs(p1_2_p0_2);
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    if( (max2 < fabs(q1_3_p0_3)) )
    {
        max2 = fabs(q1_3_p0_3);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.85816790703293534018e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (6.44428177279185717888e-15 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max3 = max1;
    if( (max3 < max2) )
    {
        max3 = max2;
    } 
    double max4 = max2;
    if( (max4 < fabs(p2_0_p0_0)) )
    {
        max4 = fabs(p2_0_p0_0);
    } 
    if( (max4 < fabs(p2_2_p0_2)) )
    {
        max4 = fabs(p2_2_p0_2);
    } 
    if( (max4 < fabs(p2_1_p0_1)) )
    {
        max4 = fabs(p2_1_p0_1);
    } 
    if( (max4 < fabs(p2_3_p0_3)) )
    {
        max4 = fabs(p2_3_p0_3);
    } 
    if( (max3 < max4) )
    {
        max3 = max4;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max3;
    upper_bound_1 = max3;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    if( (lower_bound_1 < 1.89528395402941802921e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.72443682410932010423e-13 * (((max1 * max4) * max4) * max3));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side2_6d_filter( const double* p0, const double* p1, const double* p2, const double* q0, const double* q1) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double l1;
    l1 = (1 * ((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double l2;
    l2 = (1 * ((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double a10;
    a10 = (2 * ((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double a11;
    a11 = (2 * ((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)));
    double a20;
    a20 = (2 * ((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)));
    double a21;
    a21 = (2 * ((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)));
    double Delta;
    Delta = (a11 - a10);
    double DeltaLambda0;
    DeltaLambda0 = (a11 - l1);
    double DeltaLambda1;
    DeltaLambda1 = (l1 - a10);
    double r;
    r = (((Delta * l2) - (a20 * DeltaLambda0)) - (a21 * DeltaLambda1));
    double eps;
    double max1 = fabs(p1_1_p0_1);
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    if( (max2 < fabs(q1_3_p0_3)) )
    {
        max2 = fabs(q1_3_p0_3);
    } 
    if( (max2 < fabs(q1_4_p0_4)) )
    {
        max2 = fabs(q1_4_p0_4);
    } 
    if( (max2 < fabs(q1_5_p0_5)) )
    {
        max2 = fabs(q1_5_p0_5);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.41511993781011659868e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.11111223981318615596e-14 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max3 = max1;
    if( (max3 < max2) )
    {
        max3 = max2;
    } 
    double max4 = max2;
    if( (max4 < fabs(p2_2_p0_2)) )
    {
        max4 = fabs(p2_2_p0_2);
    } 
    if( (max4 < fabs(p2_4_p0_4)) )
    {
        max4 = fabs(p2_4_p0_4);
    } 
    if( (max4 < fabs(p2_3_p0_3)) )
    {
        max4 = fabs(p2_3_p0_3);
    } 
    if( (max4 < fabs(p2_0_p0_0)) )
    {
        max4 = fabs(p2_0_p0_0);
    } 
    if( (max4 < fabs(p2_1_p0_1)) )
    {
        max4 = fabs(p2_1_p0_1);
    } 
    if( (max4 < fabs(p2_5_p0_5)) )
    {
        max4 = fabs(p2_5_p0_5);
    } 
    if( (max3 < max4) )
    {
        max3 = max4;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    if( (lower_bound_1 < 1.49958502193059513986e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.40007476026584016994e-13 * (((max1 * max4) * max4) * max3));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side2_7d_filter( const double* p0, const double* p1, const double* p2, const double* q0, const double* q1) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double l1;
    l1 = (1 * (((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double l2;
    l2 = (1 * (((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double a10;
    a10 = (2 * (((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double a11;
    a11 = (2 * (((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)));
    double a20;
    a20 = (2 * (((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)));
    double a21;
    a21 = (2 * (((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)));
    double Delta;
    Delta = (a11 - a10);
    double DeltaLambda0;
    DeltaLambda0 = (a11 - l1);
    double DeltaLambda1;
    DeltaLambda1 = (l1 - a10);
    double r;
    r = (((Delta * l2) - (a20 * DeltaLambda0)) - (a21 * DeltaLambda1));
    double eps;
    double max1 = fabs(p1_2_p0_2);
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    if( (max2 < fabs(q0_6_p0_6)) )
    {
        max2 = fabs(q0_6_p0_6);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    if( (max2 < fabs(q1_3_p0_3)) )
    {
        max2 = fabs(q1_3_p0_3);
    } 
    if( (max2 < fabs(q1_4_p0_4)) )
    {
        max2 = fabs(q1_4_p0_4);
    } 
    if( (max2 < fabs(q1_5_p0_5)) )
    {
        max2 = fabs(q1_5_p0_5);
    } 
    if( (max2 < fabs(q1_6_p0_6)) )
    {
        max2 = fabs(q1_6_p0_6);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.27080861580266953580e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.37779349582504943796e-14 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max3 = max1;
    if( (max3 < max2) )
    {
        max3 = max2;
    } 
    double max4 = max2;
    if( (max4 < fabs(p2_0_p0_0)) )
    {
        max4 = fabs(p2_0_p0_0);
    } 
    if( (max4 < fabs(p2_1_p0_1)) )
    {
        max4 = fabs(p2_1_p0_1);
    } 
    if( (max4 < fabs(p2_2_p0_2)) )
    {
        max4 = fabs(p2_2_p0_2);
    } 
    if( (max4 < fabs(p2_3_p0_3)) )
    {
        max4 = fabs(p2_3_p0_3);
    } 
    if( (max4 < fabs(p2_4_p0_4)) )
    {
        max4 = fabs(p2_4_p0_4);
    } 
    if( (max4 < fabs(p2_5_p0_5)) )
    {
        max4 = fabs(p2_5_p0_5);
    } 
    if( (max4 < fabs(p2_6_p0_6)) )
    {
        max4 = fabs(p2_6_p0_6);
    } 
    if( (max3 < max4) )
    {
        max3 = max4;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    if( (lower_bound_1 < 1.36918881183883509035e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (6.33127335329798996022e-13 * (((max1 * max4) * max4) * max3));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side2_8d_filter( const double* p0, const double* p1, const double* p2, const double* q0, const double* q1) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double p1_7_p0_7 = (p1[7] - p0[7]);
    double l1;
    l1 = (1 * ((((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)) + (p1_7_p0_7 * p1_7_p0_7)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double p2_7_p0_7 = (p2[7] - p0[7]);
    double l2;
    l2 = (1 * ((((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)) + (p2_7_p0_7 * p2_7_p0_7)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double q0_7_p0_7 = (q0[7] - p0[7]);
    double a10;
    a10 = (2 * ((((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)) + (p1_7_p0_7 * q0_7_p0_7)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double q1_7_p0_7 = (q1[7] - p0[7]);
    double a11;
    a11 = (2 * ((((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)) + (p1_7_p0_7 * q1_7_p0_7)));
    double a20;
    a20 = (2 * ((((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)) + (p2_7_p0_7 * q0_7_p0_7)));
    double a21;
    a21 = (2 * ((((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)) + (p2_7_p0_7 * q1_7_p0_7)));
    double Delta;
    Delta = (a11 - a10);
    double DeltaLambda0;
    DeltaLambda0 = (a11 - l1);
    double DeltaLambda1;
    DeltaLambda1 = (l1 - a10);
    double r;
    r = (((Delta * l2) - (a20 * DeltaLambda0)) - (a21 * DeltaLambda1));
    double eps;
    double max1 = fabs(p1_4_p0_4);
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_7_p0_7)) )
    {
        max1 = fabs(p1_7_p0_7);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q0_2_p0_2)) )
    {
        max2 = fabs(q0_2_p0_2);
    } 
    if( (max2 < fabs(q0_3_p0_3)) )
    {
        max2 = fabs(q0_3_p0_3);
    } 
    if( (max2 < fabs(q0_4_p0_4)) )
    {
        max2 = fabs(q0_4_p0_4);
    } 
    if( (max2 < fabs(q0_5_p0_5)) )
    {
        max2 = fabs(q0_5_p0_5);
    } 
    if( (max2 < fabs(q0_6_p0_6)) )
    {
        max2 = fabs(q0_6_p0_6);
    } 
    if( (max2 < fabs(q0_7_p0_7)) )
    {
        max2 = fabs(q0_7_p0_7);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    if( (max2 < fabs(q1_3_p0_3)) )
    {
        max2 = fabs(q1_3_p0_3);
    } 
    if( (max2 < fabs(q1_4_p0_4)) )
    {
        max2 = fabs(q1_4_p0_4);
    } 
    if( (max2 < fabs(q1_5_p0_5)) )
    {
        max2 = fabs(q1_5_p0_5);
    } 
    if( (max2 < fabs(q1_6_p0_6)) )
    {
        max2 = fabs(q1_6_p0_6);
    } 
    if( (max2 < fabs(q1_7_p0_7)) )
    {
        max2 = fabs(q1_7_p0_7);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (lower_bound_1 < 1.15542931091530087067e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.66670090166682227006e-14 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max3 = max2;
    if( (max3 < max1) )
    {
        max3 = max1;
    } 
    double max4 = max2;
    if( (max4 < fabs(p2_4_p0_4)) )
    {
        max4 = fabs(p2_4_p0_4);
    } 
    if( (max4 < fabs(p2_2_p0_2)) )
    {
        max4 = fabs(p2_2_p0_2);
    } 
    if( (max4 < fabs(p2_0_p0_0)) )
    {
        max4 = fabs(p2_0_p0_0);
    } 
    if( (max4 < fabs(p2_1_p0_1)) )
    {
        max4 = fabs(p2_1_p0_1);
    } 
    if( (max4 < fabs(p2_3_p0_3)) )
    {
        max4 = fabs(p2_3_p0_3);
    } 
    if( (max4 < fabs(p2_5_p0_5)) )
    {
        max4 = fabs(p2_5_p0_5);
    } 
    if( (max4 < fabs(p2_6_p0_6)) )
    {
        max4 = fabs(p2_6_p0_6);
    } 
    if( (max4 < fabs(p2_7_p0_7)) )
    {
        max4 = fabs(p2_7_p0_7);
    } 
    if( (max3 < max4) )
    {
        max3 = max4;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    if( (lower_bound_1 < 1.26419510663115923609e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.87072209578355531992e+50) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.71140112255785451890e-13 * (((max1 * max4) * max4) * max3));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


/******* extracted from ../numerics/predicates/side3.h *******/

inline int side3_2d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double l1;
    l1 = (1 * ((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double l2;
    l2 = (1 * ((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double l3;
    l3 = (1 * ((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double a10;
    a10 = (2 * ((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double a11;
    a11 = (2 * ((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double a12;
    a12 = (2 * ((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)));
    double a20;
    a20 = (2 * ((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)));
    double a21;
    a21 = (2 * ((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)));
    double a22;
    a22 = (2 * ((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)));
    double a30;
    a30 = (2 * ((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)));
    double a31;
    a31 = (2 * ((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)));
    double a32;
    a32 = (2 * ((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p2_0_p0_0);
    if( (max1 < fabs(p2_1_p0_1)) )
    {
        max1 = fabs(p2_1_p0_1);
    } 
    double max2 = fabs(q0_0_p0_0);
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q2_0_p0_0)) )
    {
        max2 = fabs(q2_0_p0_0);
    } 
    if( (max2 < fabs(q2_1_p0_1)) )
    {
        max2 = fabs(q2_1_p0_1);
    } 
    double max3 = fabs(p1_0_p0_0);
    if( (max3 < fabs(p1_1_p0_1)) )
    {
        max3 = fabs(p1_1_p0_1);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 2.79532528033945620759e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (3.64430756537603111258e-14 * (((max3 * max2) * max1) * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max2;
    if( (max4 < max1) )
    {
        max4 = max1;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    double max5 = max2;
    if( (max5 < max3) )
    {
        max5 = max3;
    } 
    double max6 = max2;
    if( (max6 < max3) )
    {
        max6 = max3;
    } 
    if( (max5 < max6) )
    {
        max5 = max6;
    } 
    double max7 = max3;
    if( (max7 < fabs(p3_1_p0_1)) )
    {
        max7 = fabs(p3_1_p0_1);
    } 
    if( (max7 < fabs(p3_0_p0_0)) )
    {
        max7 = fabs(p3_0_p0_0);
    } 
    if( (max5 < max7) )
    {
        max5 = max7;
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    if( (max4 < max7) )
    {
        max4 = max7;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    if( (lower_bound_1 < 6.01986729486167248087e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.67544471613800658534e-13 * (((((max7 * max2) * max1) * max6) * max5) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side3_3d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double l1;
    l1 = (1 * (((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double l2;
    l2 = (1 * (((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double l3;
    l3 = (1 * (((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double a10;
    a10 = (2 * (((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double a11;
    a11 = (2 * (((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double a12;
    a12 = (2 * (((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)));
    double a20;
    a20 = (2 * (((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)));
    double a21;
    a21 = (2 * (((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)));
    double a22;
    a22 = (2 * (((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)));
    double a30;
    a30 = (2 * (((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)));
    double a31;
    a31 = (2 * (((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)));
    double a32;
    a32 = (2 * (((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p1_1_p0_1);
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    double max2 = fabs(q0_2_p0_2);
    if( (max2 < fabs(q0_0_p0_0)) )
    {
        max2 = fabs(q0_0_p0_0);
    } 
    if( (max2 < fabs(q0_1_p0_1)) )
    {
        max2 = fabs(q0_1_p0_1);
    } 
    if( (max2 < fabs(q1_0_p0_0)) )
    {
        max2 = fabs(q1_0_p0_0);
    } 
    if( (max2 < fabs(q1_1_p0_1)) )
    {
        max2 = fabs(q1_1_p0_1);
    } 
    if( (max2 < fabs(q1_2_p0_2)) )
    {
        max2 = fabs(q1_2_p0_2);
    } 
    if( (max2 < fabs(q2_0_p0_0)) )
    {
        max2 = fabs(q2_0_p0_0);
    } 
    if( (max2 < fabs(q2_1_p0_1)) )
    {
        max2 = fabs(q2_1_p0_1);
    } 
    if( (max2 < fabs(q2_2_p0_2)) )
    {
        max2 = fabs(q2_2_p0_2);
    } 
    double max3 = fabs(p2_2_p0_2);
    if( (max3 < fabs(p2_0_p0_0)) )
    {
        max3 = fabs(p2_0_p0_0);
    } 
    if( (max3 < fabs(p2_1_p0_1)) )
    {
        max3 = fabs(p2_1_p0_1);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 2.22985945097100191780e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.99983341597279045654e-14 * (((max1 * max2) * max3) * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max1;
    if( (max4 < max2) )
    {
        max4 = max2;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    double max5 = max1;
    if( (max5 < max2) )
    {
        max5 = max2;
    } 
    double max6 = max1;
    if( (max6 < fabs(p3_0_p0_0)) )
    {
        max6 = fabs(p3_0_p0_0);
    } 
    if( (max6 < fabs(p3_1_p0_1)) )
    {
        max6 = fabs(p3_1_p0_1);
    } 
    if( (max6 < fabs(p3_2_p0_2)) )
    {
        max6 = fabs(p3_2_p0_2);
    } 
    if( (max5 < max6) )
    {
        max5 = max6;
    } 
    double max7 = max1;
    if( (max7 < max2) )
    {
        max7 = max2;
    } 
    if( (max5 < max7) )
    {
        max5 = max7;
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    if( (max4 < max7) )
    {
        max4 = max7;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    if( (lower_bound_1 < 4.84416636653081796592e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.72198804259438718181e-12 * (((((max6 * max2) * max3) * max7) * max5) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side3_4d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double l1;
    l1 = (1 * ((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double l2;
    l2 = (1 * ((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double l3;
    l3 = (1 * ((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double a10;
    a10 = (2 * ((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double a11;
    a11 = (2 * ((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double a12;
    a12 = (2 * ((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)));
    double a20;
    a20 = (2 * ((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)));
    double a21;
    a21 = (2 * ((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)));
    double a22;
    a22 = (2 * ((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)));
    double a30;
    a30 = (2 * ((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)));
    double a31;
    a31 = (2 * ((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)));
    double a32;
    a32 = (2 * ((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p1_3_p0_3);
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    double max2 = fabs(p2_3_p0_3);
    if( (max2 < fabs(p2_2_p0_2)) )
    {
        max2 = fabs(p2_2_p0_2);
    } 
    if( (max2 < fabs(p2_0_p0_0)) )
    {
        max2 = fabs(p2_0_p0_0);
    } 
    if( (max2 < fabs(p2_1_p0_1)) )
    {
        max2 = fabs(p2_1_p0_1);
    } 
    double max3 = fabs(q0_1_p0_1);
    if( (max3 < fabs(q0_0_p0_0)) )
    {
        max3 = fabs(q0_0_p0_0);
    } 
    if( (max3 < fabs(q0_2_p0_2)) )
    {
        max3 = fabs(q0_2_p0_2);
    } 
    if( (max3 < fabs(q0_3_p0_3)) )
    {
        max3 = fabs(q0_3_p0_3);
    } 
    if( (max3 < fabs(q1_0_p0_0)) )
    {
        max3 = fabs(q1_0_p0_0);
    } 
    if( (max3 < fabs(q1_1_p0_1)) )
    {
        max3 = fabs(q1_1_p0_1);
    } 
    if( (max3 < fabs(q1_2_p0_2)) )
    {
        max3 = fabs(q1_2_p0_2);
    } 
    if( (max3 < fabs(q1_3_p0_3)) )
    {
        max3 = fabs(q1_3_p0_3);
    } 
    if( (max3 < fabs(q2_0_p0_0)) )
    {
        max3 = fabs(q2_0_p0_0);
    } 
    if( (max3 < fabs(q2_1_p0_1)) )
    {
        max3 = fabs(q2_1_p0_1);
    } 
    if( (max3 < fabs(q2_2_p0_2)) )
    {
        max3 = fabs(q2_2_p0_2);
    } 
    if( (max3 < fabs(q2_3_p0_3)) )
    {
        max3 = fabs(q2_3_p0_3);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.89528395402941802921e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.72443682410931985179e-13 * (((max1 * max3) * max2) * max3));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max1;
    double max5 = max1;
    double max6 = max1;
    if( (max6 < fabs(p3_0_p0_0)) )
    {
        max6 = fabs(p3_0_p0_0);
    } 
    if( (max6 < fabs(p3_3_p0_3)) )
    {
        max6 = fabs(p3_3_p0_3);
    } 
    if( (max6 < fabs(p3_2_p0_2)) )
    {
        max6 = fabs(p3_2_p0_2);
    } 
    if( (max6 < fabs(p3_1_p0_1)) )
    {
        max6 = fabs(p3_1_p0_1);
    } 
    if( (max5 < max6) )
    {
        max5 = max6;
    } 
    if( (max5 < max3) )
    {
        max5 = max3;
    } 
    double max7 = max1;
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    if( (max5 < max7) )
    {
        max5 = max7;
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    if( (max4 < max2) )
    {
        max4 = max2;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    if( (max4 < max7) )
    {
        max4 = max7;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max5;
    upper_bound_1 = max5;
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    if( (lower_bound_1 < 4.14607644401726239868e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.38046888801178809320e-12 * (((((max6 * max3) * max2) * max7) * max5) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side3_6d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double l1;
    l1 = (1 * ((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double l2;
    l2 = (1 * ((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double l3;
    l3 = (1 * ((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double a10;
    a10 = (2 * ((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double a11;
    a11 = (2 * ((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double a12;
    a12 = (2 * ((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)));
    double a20;
    a20 = (2 * ((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)));
    double a21;
    a21 = (2 * ((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)));
    double a22;
    a22 = (2 * ((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)));
    double a30;
    a30 = (2 * ((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)));
    double a31;
    a31 = (2 * ((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)));
    double a32;
    a32 = (2 * ((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p1_0_p0_0);
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    double max2 = fabs(p2_0_p0_0);
    if( (max2 < fabs(p2_1_p0_1)) )
    {
        max2 = fabs(p2_1_p0_1);
    } 
    if( (max2 < fabs(p2_2_p0_2)) )
    {
        max2 = fabs(p2_2_p0_2);
    } 
    if( (max2 < fabs(p2_3_p0_3)) )
    {
        max2 = fabs(p2_3_p0_3);
    } 
    if( (max2 < fabs(p2_4_p0_4)) )
    {
        max2 = fabs(p2_4_p0_4);
    } 
    if( (max2 < fabs(p2_5_p0_5)) )
    {
        max2 = fabs(p2_5_p0_5);
    } 
    double max3 = fabs(q0_0_p0_0);
    if( (max3 < fabs(q0_1_p0_1)) )
    {
        max3 = fabs(q0_1_p0_1);
    } 
    if( (max3 < fabs(q0_2_p0_2)) )
    {
        max3 = fabs(q0_2_p0_2);
    } 
    if( (max3 < fabs(q0_3_p0_3)) )
    {
        max3 = fabs(q0_3_p0_3);
    } 
    if( (max3 < fabs(q0_4_p0_4)) )
    {
        max3 = fabs(q0_4_p0_4);
    } 
    if( (max3 < fabs(q0_5_p0_5)) )
    {
        max3 = fabs(q0_5_p0_5);
    } 
    if( (max3 < fabs(q1_0_p0_0)) )
    {
        max3 = fabs(q1_0_p0_0);
    } 
    if( (max3 < fabs(q1_1_p0_1)) )
    {
        max3 = fabs(q1_1_p0_1);
    } 
    if( (max3 < fabs(q1_2_p0_2)) )
    {
        max3 = fabs(q1_2_p0_2);
    } 
    if( (max3 < fabs(q1_3_p0_3)) )
    {
        max3 = fabs(q1_3_p0_3);
    } 
    if( (max3 < fabs(q1_4_p0_4)) )
    {
        max3 = fabs(q1_4_p0_4);
    } 
    if( (max3 < fabs(q1_5_p0_5)) )
    {
        max3 = fabs(q1_5_p0_5);
    } 
    if( (max3 < fabs(q2_0_p0_0)) )
    {
        max3 = fabs(q2_0_p0_0);
    } 
    if( (max3 < fabs(q2_1_p0_1)) )
    {
        max3 = fabs(q2_1_p0_1);
    } 
    if( (max3 < fabs(q2_2_p0_2)) )
    {
        max3 = fabs(q2_2_p0_2);
    } 
    if( (max3 < fabs(q2_3_p0_3)) )
    {
        max3 = fabs(q2_3_p0_3);
    } 
    if( (max3 < fabs(q2_4_p0_4)) )
    {
        max3 = fabs(q2_4_p0_4);
    } 
    if( (max3 < fabs(q2_5_p0_5)) )
    {
        max3 = fabs(q2_5_p0_5);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.49958502193059513986e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.40007476026583916019e-13 * (((max1 * max3) * max2) * max3));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max1;
    if( (max4 < max2) )
    {
        max4 = max2;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    double max5 = max1;
    if( (max5 < fabs(p3_1_p0_1)) )
    {
        max5 = fabs(p3_1_p0_1);
    } 
    if( (max5 < fabs(p3_2_p0_2)) )
    {
        max5 = fabs(p3_2_p0_2);
    } 
    if( (max5 < fabs(p3_0_p0_0)) )
    {
        max5 = fabs(p3_0_p0_0);
    } 
    if( (max5 < fabs(p3_3_p0_3)) )
    {
        max5 = fabs(p3_3_p0_3);
    } 
    if( (max5 < fabs(p3_4_p0_4)) )
    {
        max5 = fabs(p3_4_p0_4);
    } 
    if( (max5 < fabs(p3_5_p0_5)) )
    {
        max5 = fabs(p3_5_p0_5);
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    double max6 = max1;
    if( (max6 < max3) )
    {
        max6 = max3;
    } 
    if( (max6 < max5) )
    {
        max6 = max5;
    } 
    double max7 = max1;
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    if( (max6 < max7) )
    {
        max6 = max7;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    if( (max4 < max7) )
    {
        max4 = max7;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    if( (lower_bound_1 < 3.31864264949884013629e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.66564133587113197628e-11 * (((((max5 * max3) * max2) * max7) * max6) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side3_7d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double l1;
    l1 = (1 * (((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double l2;
    l2 = (1 * (((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double p3_6_p0_6 = (p3[6] - p0[6]);
    double l3;
    l3 = (1 * (((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)) + (p3_6_p0_6 * p3_6_p0_6)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double a10;
    a10 = (2 * (((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double a11;
    a11 = (2 * (((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double q2_6_p0_6 = (q2[6] - p0[6]);
    double a12;
    a12 = (2 * (((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)) + (p1_6_p0_6 * q2_6_p0_6)));
    double a20;
    a20 = (2 * (((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)));
    double a21;
    a21 = (2 * (((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)));
    double a22;
    a22 = (2 * (((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)) + (p2_6_p0_6 * q2_6_p0_6)));
    double a30;
    a30 = (2 * (((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)) + (p3_6_p0_6 * q0_6_p0_6)));
    double a31;
    a31 = (2 * (((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)) + (p3_6_p0_6 * q1_6_p0_6)));
    double a32;
    a32 = (2 * (((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)) + (p3_6_p0_6 * q2_6_p0_6)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p1_1_p0_1);
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    double max2 = fabs(p2_0_p0_0);
    if( (max2 < fabs(p2_1_p0_1)) )
    {
        max2 = fabs(p2_1_p0_1);
    } 
    if( (max2 < fabs(p2_2_p0_2)) )
    {
        max2 = fabs(p2_2_p0_2);
    } 
    if( (max2 < fabs(p2_3_p0_3)) )
    {
        max2 = fabs(p2_3_p0_3);
    } 
    if( (max2 < fabs(p2_4_p0_4)) )
    {
        max2 = fabs(p2_4_p0_4);
    } 
    if( (max2 < fabs(p2_5_p0_5)) )
    {
        max2 = fabs(p2_5_p0_5);
    } 
    if( (max2 < fabs(p2_6_p0_6)) )
    {
        max2 = fabs(p2_6_p0_6);
    } 
    double max3 = fabs(q0_0_p0_0);
    if( (max3 < fabs(q0_1_p0_1)) )
    {
        max3 = fabs(q0_1_p0_1);
    } 
    if( (max3 < fabs(q0_2_p0_2)) )
    {
        max3 = fabs(q0_2_p0_2);
    } 
    if( (max3 < fabs(q0_3_p0_3)) )
    {
        max3 = fabs(q0_3_p0_3);
    } 
    if( (max3 < fabs(q0_4_p0_4)) )
    {
        max3 = fabs(q0_4_p0_4);
    } 
    if( (max3 < fabs(q0_5_p0_5)) )
    {
        max3 = fabs(q0_5_p0_5);
    } 
    if( (max3 < fabs(q0_6_p0_6)) )
    {
        max3 = fabs(q0_6_p0_6);
    } 
    if( (max3 < fabs(q1_0_p0_0)) )
    {
        max3 = fabs(q1_0_p0_0);
    } 
    if( (max3 < fabs(q1_1_p0_1)) )
    {
        max3 = fabs(q1_1_p0_1);
    } 
    if( (max3 < fabs(q1_2_p0_2)) )
    {
        max3 = fabs(q1_2_p0_2);
    } 
    if( (max3 < fabs(q1_3_p0_3)) )
    {
        max3 = fabs(q1_3_p0_3);
    } 
    if( (max3 < fabs(q1_4_p0_4)) )
    {
        max3 = fabs(q1_4_p0_4);
    } 
    if( (max3 < fabs(q1_5_p0_5)) )
    {
        max3 = fabs(q1_5_p0_5);
    } 
    if( (max3 < fabs(q1_6_p0_6)) )
    {
        max3 = fabs(q1_6_p0_6);
    } 
    if( (max3 < fabs(q2_0_p0_0)) )
    {
        max3 = fabs(q2_0_p0_0);
    } 
    if( (max3 < fabs(q2_1_p0_1)) )
    {
        max3 = fabs(q2_1_p0_1);
    } 
    if( (max3 < fabs(q2_2_p0_2)) )
    {
        max3 = fabs(q2_2_p0_2);
    } 
    if( (max3 < fabs(q2_3_p0_3)) )
    {
        max3 = fabs(q2_3_p0_3);
    } 
    if( (max3 < fabs(q2_4_p0_4)) )
    {
        max3 = fabs(q2_4_p0_4);
    } 
    if( (max3 < fabs(q2_5_p0_5)) )
    {
        max3 = fabs(q2_5_p0_5);
    } 
    if( (max3 < fabs(q2_6_p0_6)) )
    {
        max3 = fabs(q2_6_p0_6);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max2;
    upper_bound_1 = max2;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.36918881183883509035e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (6.33127335329798996022e-13 * (((max1 * max3) * max2) * max3));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4;
    double max7 = max1;
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    max4 = max7;
    if( (max4 < max2) )
    {
        max4 = max2;
    } 
    double max5 = max1;
    if( (max5 < fabs(p3_0_p0_0)) )
    {
        max5 = fabs(p3_0_p0_0);
    } 
    if( (max5 < fabs(p3_1_p0_1)) )
    {
        max5 = fabs(p3_1_p0_1);
    } 
    if( (max5 < fabs(p3_2_p0_2)) )
    {
        max5 = fabs(p3_2_p0_2);
    } 
    if( (max5 < fabs(p3_3_p0_3)) )
    {
        max5 = fabs(p3_3_p0_3);
    } 
    if( (max5 < fabs(p3_4_p0_4)) )
    {
        max5 = fabs(p3_4_p0_4);
    } 
    if( (max5 < fabs(p3_5_p0_5)) )
    {
        max5 = fabs(p3_5_p0_5);
    } 
    if( (max5 < fabs(p3_6_p0_6)) )
    {
        max5 = fabs(p3_6_p0_6);
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    if( (max4 < max1) )
    {
        max4 = max1;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    double max6 = max7;
    if( (max6 < max5) )
    {
        max6 = max5;
    } 
    if( (max6 < max1) )
    {
        max6 = max1;
    } 
    if( (max6 < max3) )
    {
        max6 = max3;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max7;
    upper_bound_1 = max7;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (lower_bound_1 < 3.04548303565602498901e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (2.78873548804336160566e-11 * (((((max5 * max3) * max2) * max7) * max6) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


inline int side3_8d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* q0, const double* q1, const double* q2) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double p1_7_p0_7 = (p1[7] - p0[7]);
    double l1;
    l1 = (1 * ((((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)) + (p1_7_p0_7 * p1_7_p0_7)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double p2_7_p0_7 = (p2[7] - p0[7]);
    double l2;
    l2 = (1 * ((((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)) + (p2_7_p0_7 * p2_7_p0_7)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double p3_6_p0_6 = (p3[6] - p0[6]);
    double p3_7_p0_7 = (p3[7] - p0[7]);
    double l3;
    l3 = (1 * ((((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)) + (p3_6_p0_6 * p3_6_p0_6)) + (p3_7_p0_7 * p3_7_p0_7)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double q0_7_p0_7 = (q0[7] - p0[7]);
    double a10;
    a10 = (2 * ((((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)) + (p1_7_p0_7 * q0_7_p0_7)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double q1_7_p0_7 = (q1[7] - p0[7]);
    double a11;
    a11 = (2 * ((((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)) + (p1_7_p0_7 * q1_7_p0_7)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double q2_6_p0_6 = (q2[6] - p0[6]);
    double q2_7_p0_7 = (q2[7] - p0[7]);
    double a12;
    a12 = (2 * ((((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)) + (p1_6_p0_6 * q2_6_p0_6)) + (p1_7_p0_7 * q2_7_p0_7)));
    double a20;
    a20 = (2 * ((((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)) + (p2_7_p0_7 * q0_7_p0_7)));
    double a21;
    a21 = (2 * ((((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)) + (p2_7_p0_7 * q1_7_p0_7)));
    double a22;
    a22 = (2 * ((((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)) + (p2_6_p0_6 * q2_6_p0_6)) + (p2_7_p0_7 * q2_7_p0_7)));
    double a30;
    a30 = (2 * ((((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)) + (p3_6_p0_6 * q0_6_p0_6)) + (p3_7_p0_7 * q0_7_p0_7)));
    double a31;
    a31 = (2 * ((((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)) + (p3_6_p0_6 * q1_6_p0_6)) + (p3_7_p0_7 * q1_7_p0_7)));
    double a32;
    a32 = (2 * ((((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)) + (p3_6_p0_6 * q2_6_p0_6)) + (p3_7_p0_7 * q2_7_p0_7)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(p2_1_p0_1);
    if( (max1 < fabs(p2_0_p0_0)) )
    {
        max1 = fabs(p2_0_p0_0);
    } 
    if( (max1 < fabs(p2_3_p0_3)) )
    {
        max1 = fabs(p2_3_p0_3);
    } 
    if( (max1 < fabs(p2_2_p0_2)) )
    {
        max1 = fabs(p2_2_p0_2);
    } 
    if( (max1 < fabs(p2_4_p0_4)) )
    {
        max1 = fabs(p2_4_p0_4);
    } 
    if( (max1 < fabs(p2_7_p0_7)) )
    {
        max1 = fabs(p2_7_p0_7);
    } 
    if( (max1 < fabs(p2_5_p0_5)) )
    {
        max1 = fabs(p2_5_p0_5);
    } 
    if( (max1 < fabs(p2_6_p0_6)) )
    {
        max1 = fabs(p2_6_p0_6);
    } 
    double max2 = fabs(p1_4_p0_4);
    if( (max2 < fabs(p1_1_p0_1)) )
    {
        max2 = fabs(p1_1_p0_1);
    } 
    if( (max2 < fabs(p1_0_p0_0)) )
    {
        max2 = fabs(p1_0_p0_0);
    } 
    if( (max2 < fabs(p1_3_p0_3)) )
    {
        max2 = fabs(p1_3_p0_3);
    } 
    if( (max2 < fabs(p1_2_p0_2)) )
    {
        max2 = fabs(p1_2_p0_2);
    } 
    if( (max2 < fabs(p1_5_p0_5)) )
    {
        max2 = fabs(p1_5_p0_5);
    } 
    if( (max2 < fabs(p1_6_p0_6)) )
    {
        max2 = fabs(p1_6_p0_6);
    } 
    if( (max2 < fabs(p1_7_p0_7)) )
    {
        max2 = fabs(p1_7_p0_7);
    } 
    double max3 = fabs(q0_0_p0_0);
    if( (max3 < fabs(q0_1_p0_1)) )
    {
        max3 = fabs(q0_1_p0_1);
    } 
    if( (max3 < fabs(q0_2_p0_2)) )
    {
        max3 = fabs(q0_2_p0_2);
    } 
    if( (max3 < fabs(q0_3_p0_3)) )
    {
        max3 = fabs(q0_3_p0_3);
    } 
    if( (max3 < fabs(q0_4_p0_4)) )
    {
        max3 = fabs(q0_4_p0_4);
    } 
    if( (max3 < fabs(q0_5_p0_5)) )
    {
        max3 = fabs(q0_5_p0_5);
    } 
    if( (max3 < fabs(q0_6_p0_6)) )
    {
        max3 = fabs(q0_6_p0_6);
    } 
    if( (max3 < fabs(q0_7_p0_7)) )
    {
        max3 = fabs(q0_7_p0_7);
    } 
    if( (max3 < fabs(q1_0_p0_0)) )
    {
        max3 = fabs(q1_0_p0_0);
    } 
    if( (max3 < fabs(q1_1_p0_1)) )
    {
        max3 = fabs(q1_1_p0_1);
    } 
    if( (max3 < fabs(q1_2_p0_2)) )
    {
        max3 = fabs(q1_2_p0_2);
    } 
    if( (max3 < fabs(q1_3_p0_3)) )
    {
        max3 = fabs(q1_3_p0_3);
    } 
    if( (max3 < fabs(q1_4_p0_4)) )
    {
        max3 = fabs(q1_4_p0_4);
    } 
    if( (max3 < fabs(q1_5_p0_5)) )
    {
        max3 = fabs(q1_5_p0_5);
    } 
    if( (max3 < fabs(q1_6_p0_6)) )
    {
        max3 = fabs(q1_6_p0_6);
    } 
    if( (max3 < fabs(q1_7_p0_7)) )
    {
        max3 = fabs(q1_7_p0_7);
    } 
    if( (max3 < fabs(q2_0_p0_0)) )
    {
        max3 = fabs(q2_0_p0_0);
    } 
    if( (max3 < fabs(q2_1_p0_1)) )
    {
        max3 = fabs(q2_1_p0_1);
    } 
    if( (max3 < fabs(q2_2_p0_2)) )
    {
        max3 = fabs(q2_2_p0_2);
    } 
    if( (max3 < fabs(q2_3_p0_3)) )
    {
        max3 = fabs(q2_3_p0_3);
    } 
    if( (max3 < fabs(q2_4_p0_4)) )
    {
        max3 = fabs(q2_4_p0_4);
    } 
    if( (max3 < fabs(q2_5_p0_5)) )
    {
        max3 = fabs(q2_5_p0_5);
    } 
    if( (max3 < fabs(q2_6_p0_6)) )
    {
        max3 = fabs(q2_6_p0_6);
    } 
    if( (max3 < fabs(q2_7_p0_7)) )
    {
        max3 = fabs(q2_7_p0_7);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.26419510663115923609e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.71140112255785451890e-13 * (((max2 * max3) * max1) * max3));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max1;
    if( (max4 < max2) )
    {
        max4 = max2;
    } 
    if( (max4 < max3) )
    {
        max4 = max3;
    } 
    double max5 = max2;
    if( (max5 < fabs(p3_0_p0_0)) )
    {
        max5 = fabs(p3_0_p0_0);
    } 
    if( (max5 < fabs(p3_1_p0_1)) )
    {
        max5 = fabs(p3_1_p0_1);
    } 
    if( (max5 < fabs(p3_2_p0_2)) )
    {
        max5 = fabs(p3_2_p0_2);
    } 
    if( (max5 < fabs(p3_3_p0_3)) )
    {
        max5 = fabs(p3_3_p0_3);
    } 
    if( (max5 < fabs(p3_4_p0_4)) )
    {
        max5 = fabs(p3_4_p0_4);
    } 
    if( (max5 < fabs(p3_5_p0_5)) )
    {
        max5 = fabs(p3_5_p0_5);
    } 
    if( (max5 < fabs(p3_6_p0_6)) )
    {
        max5 = fabs(p3_6_p0_6);
    } 
    if( (max5 < fabs(p3_7_p0_7)) )
    {
        max5 = fabs(p3_7_p0_7);
    } 
    if( (max4 < max5) )
    {
        max4 = max5;
    } 
    double max6 = max2;
    if( (max6 < max3) )
    {
        max6 = max3;
    } 
    if( (max6 < max5) )
    {
        max6 = max5;
    } 
    double max7 = max2;
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    if( (max6 < max7) )
    {
        max6 = max7;
    } 
    if( (max4 < max6) )
    {
        max4 = max6;
    } 
    if( (max4 < max7) )
    {
        max4 = max7;
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    if( (lower_bound_1 < 2.82528483194754087282e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.29807421463370647479e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.37492894694731169807e-11 * (((((max5 * max3) * max1) * max7) * max6) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


/******* extracted from ../numerics/predicates/side3h.h *******/

inline int side3h_3d_filter( const double* p0, const double* p1, const double* p2, const double* p3, double h0, double h1, double h2, double h3, const double* q0, const double* q1, const double* q2) {
    double l1;
    l1 = (h1 - h0);
    double l2;
    l2 = (h2 - h0);
    double l3;
    l3 = (h3 - h0);
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double a10;
    a10 = (2 * (((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double a11;
    a11 = (2 * (((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double a12;
    a12 = (2 * (((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double a20;
    a20 = (2 * (((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)));
    double a21;
    a21 = (2 * (((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)));
    double a22;
    a22 = (2 * (((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double a30;
    a30 = (2 * (((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)));
    double a31;
    a31 = (2 * (((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)));
    double a32;
    a32 = (2 * (((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)));
    double b00;
    b00 = ((a11 * a22) - (a12 * a21));
    double b01;
    b01 = (a21 - a22);
    double b02;
    b02 = (a12 - a11);
    double b10;
    b10 = ((a12 * a20) - (a10 * a22));
    double b11;
    b11 = (a22 - a20);
    double b12;
    b12 = (a10 - a12);
    double b20;
    b20 = ((a10 * a21) - (a11 * a20));
    double b21;
    b21 = (a20 - a21);
    double b22;
    b22 = (a11 - a10);
    double Delta;
    Delta = ((b00 + b10) + b20);
    double DeltaLambda0;
    DeltaLambda0 = (((b01 * l1) + (b02 * l2)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = (((b11 * l1) + (b12 * l2)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = (((b21 * l1) + (b22 * l2)) + b20);
    double r;
    r = ((Delta * l3) - (((a30 * DeltaLambda0) + (a31 * DeltaLambda1)) + (a32 * DeltaLambda2)));
    double eps;
    double max1 = fabs(q2_2_p0_2);
    if( (max1 < fabs(q0_0_p0_0)) )
    {
        max1 = fabs(q0_0_p0_0);
    } 
    if( (max1 < fabs(q0_1_p0_1)) )
    {
        max1 = fabs(q0_1_p0_1);
    } 
    if( (max1 < fabs(q0_2_p0_2)) )
    {
        max1 = fabs(q0_2_p0_2);
    } 
    if( (max1 < fabs(q1_0_p0_0)) )
    {
        max1 = fabs(q1_0_p0_0);
    } 
    if( (max1 < fabs(q1_1_p0_1)) )
    {
        max1 = fabs(q1_1_p0_1);
    } 
    if( (max1 < fabs(q1_2_p0_2)) )
    {
        max1 = fabs(q1_2_p0_2);
    } 
    if( (max1 < fabs(q2_0_p0_0)) )
    {
        max1 = fabs(q2_0_p0_0);
    } 
    if( (max1 < fabs(q2_1_p0_1)) )
    {
        max1 = fabs(q2_1_p0_1);
    } 
    double max2 = fabs(p2_0_p0_0);
    if( (max2 < fabs(p2_1_p0_1)) )
    {
        max2 = fabs(p2_1_p0_1);
    } 
    if( (max2 < fabs(p2_2_p0_2)) )
    {
        max2 = fabs(p2_2_p0_2);
    } 
    double max3 = fabs(p1_0_p0_0);
    if( (max3 < fabs(p1_1_p0_1)) )
    {
        max3 = fabs(p1_1_p0_1);
    } 
    if( (max3 < fabs(p1_2_p0_2)) )
    {
        max3 = fabs(p1_2_p0_2);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 2.22985945097100191780e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.99983341597279045654e-14 * (((max3 * max1) * max2) * max1));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    double max4 = max2;
    if( (max4 < fabs(l1)) )
    {
        max4 = fabs(l1);
    } 
    if( (max4 < fabs(l2)) )
    {
        max4 = fabs(l2);
    } 
    double max5 = max2;
    if( (max5 < max3) )
    {
        max5 = max3;
    } 
    if( (max5 < fabs(l3)) )
    {
        max5 = fabs(l3);
    } 
    double max6 = max2;
    if( (max6 < fabs(q2_2_p0_2)) )
    {
        max6 = fabs(q2_2_p0_2);
    } 
    if( (max6 < fabs(q0_0_p0_0)) )
    {
        max6 = fabs(q0_0_p0_0);
    } 
    if( (max6 < fabs(q0_1_p0_1)) )
    {
        max6 = fabs(q0_1_p0_1);
    } 
    if( (max6 < fabs(q0_2_p0_2)) )
    {
        max6 = fabs(q0_2_p0_2);
    } 
    if( (max6 < fabs(q2_0_p0_0)) )
    {
        max6 = fabs(q2_0_p0_0);
    } 
    if( (max6 < fabs(q2_1_p0_1)) )
    {
        max6 = fabs(q2_1_p0_1);
    } 
    double max7 = max3;
    if( (max7 < fabs(p3_0_p0_0)) )
    {
        max7 = fabs(p3_0_p0_0);
    } 
    if( (max7 < fabs(p3_1_p0_1)) )
    {
        max7 = fabs(p3_1_p0_1);
    } 
    if( (max7 < fabs(p3_2_p0_2)) )
    {
        max7 = fabs(p3_2_p0_2);
    } 
    int r_sign;
    int int_tmp_result_FFWKCAA;
    lower_bound_1 = max6;
    upper_bound_1 = max6;
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (lower_bound_1 < 5.53478725478149652989e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 2.59614842926741294957e+33) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (7.73996217364502738018e-13 * (((((max7 * max1) * max6) * max1) * max5) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    r_sign = int_tmp_result_FFWKCAA;
    return (Delta_sign * r_sign);
} 


/******* extracted from ../numerics/predicates/side3_2dlifted.h *******/

inline int side3_2dlifted_2d_filter( const double* p0, const double* p1, const double* p2, const double* p3, double h0, double h1, double h2, double h3) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (h0 - h1);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (h0 - h2);
    double a31;
    a31 = (p3[0] - p0[0]);
    double a32;
    a32 = (p3[1] - p0[1]);
    double a33;
    a33 = (h0 - h3);
    double Delta1;
    Delta1 = ((a21 * a32) - (a22 * a31));
    double Delta2;
    Delta2 = ((a11 * a32) - (a12 * a31));
    double Delta3;
    Delta3 = ((a11 * a22) - (a12 * a21));
    double r;
    r = (((Delta1 * a13) - (Delta2 * a23)) + (Delta3 * a33));
    double eps;
    double max1 = fabs(a11);
    if( (max1 < fabs(a12)) )
    {
        max1 = fabs(a12);
    } 
    double max2 = fabs(a21);
    if( (max2 < fabs(a22)) )
    {
        max2 = fabs(a22);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta3_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 5.00368081960964635413e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.88720573725927976811e-16 * (max1 * max2));
        if( (Delta3 > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta3 < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta3_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max3 = max1;
    if( (max3 < max2) )
    {
        max3 = max2;
    } 
    double max4 = fabs(a13);
    if( (max4 < fabs(a23)) )
    {
        max4 = fabs(a23);
    } 
    if( (max4 < fabs(a33)) )
    {
        max4 = fabs(a33);
    } 
    double max5 = max2;
    if( (max5 < fabs(a31)) )
    {
        max5 = fabs(a31);
    } 
    if( (max5 < fabs(a32)) )
    {
        max5 = fabs(a32);
    } 
    lower_bound_1 = max3;
    upper_bound_1 = max3;
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (lower_bound_1 < 1.63288018496748314939e-98) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 5.59936185544450928309e+101) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (5.11071278299732992696e-15 * ((max3 * max5) * max4));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta3_sign * int_tmp_result_FFWKCAA);
} 

/******* extracted from ../numerics/predicates/side4.h *******/

inline int side4_3d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (p1[2] - p0[2]);
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double a14;
    a14 = -(((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2));
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (p2[2] - p0[2]);
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double a24;
    a24 = -(((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2));
    double a31;
    a31 = (p3[0] - p0[0]);
    double a32;
    a32 = (p3[1] - p0[1]);
    double a33;
    a33 = (p3[2] - p0[2]);
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double a34;
    a34 = -(((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2));
    double a41;
    a41 = (p4[0] - p0[0]);
    double a42;
    a42 = (p4[1] - p0[1]);
    double a43;
    a43 = (p4[2] - p0[2]);
    double p4_0_p0_0 = (p4[0] - p0[0]);
    double p4_1_p0_1 = (p4[1] - p0[1]);
    double p4_2_p0_2 = (p4[2] - p0[2]);
    double a44;
    a44 = -(((p4_0_p0_0 * p4_0_p0_0) + (p4_1_p0_1 * p4_1_p0_1)) + (p4_2_p0_2 * p4_2_p0_2));
    double Delta1;
    Delta1 = (((a21 * ((a32 * a43) - (a33 * a42))) - (a31 * ((a22 * a43) - (a23 * a42)))) + (a41 * ((a22 * a33) - (a23 * a32))));
    double Delta2;
    Delta2 = (((a11 * ((a32 * a43) - (a33 * a42))) - (a31 * ((a12 * a43) - (a13 * a42)))) + (a41 * ((a12 * a33) - (a13 * a32))));
    double Delta3;
    Delta3 = (((a11 * ((a22 * a43) - (a23 * a42))) - (a21 * ((a12 * a43) - (a13 * a42)))) + (a41 * ((a12 * a23) - (a13 * a22))));
    double Delta4;
    Delta4 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double r;
    r = ((((Delta1 * a14) - (Delta2 * a24)) + (Delta3 * a34)) - (Delta4 * a44));
    double eps;
    double max1 = fabs(a11);
    if( (max1 < fabs(a21)) )
    {
        max1 = fabs(a21);
    } 
    if( (max1 < fabs(a31)) )
    {
        max1 = fabs(a31);
    } 
    double max2 = fabs(a12);
    if( (max2 < fabs(a13)) )
    {
        max2 = fabs(a13);
    } 
    if( (max2 < fabs(a22)) )
    {
        max2 = fabs(a22);
    } 
    if( (max2 < fabs(a23)) )
    {
        max2 = fabs(a23);
    } 
    double max3 = fabs(a22);
    if( (max3 < fabs(a23)) )
    {
        max3 = fabs(a23);
    } 
    if( (max3 < fabs(a32)) )
    {
        max3 = fabs(a32);
    } 
    if( (max3 < fabs(a33)) )
    {
        max3 = fabs(a33);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta4_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 1.63288018496748314939e-98) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 3.21387608851797948065e+60) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (5.11071278299732992696e-15 * ((max2 * max3) * max1));
        if( (Delta4 > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta4 < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta4_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max4 = max1;
    if( (max4 < fabs(a41)) )
    {
        max4 = fabs(a41);
    } 
    double max5 = max3;
    if( (max5 < max2) )
    {
        max5 = max2;
    } 
    double max6 = max3;
    if( (max6 < fabs(a42)) )
    {
        max6 = fabs(a42);
    } 
    if( (max6 < fabs(a43)) )
    {
        max6 = fabs(a43);
    } 
    double max7 = fabs(p1_0_p0_0);
    if( (max7 < fabs(p1_1_p0_1)) )
    {
        max7 = fabs(p1_1_p0_1);
    } 
    if( (max7 < fabs(p1_2_p0_2)) )
    {
        max7 = fabs(p1_2_p0_2);
    } 
    if( (max7 < fabs(p2_0_p0_0)) )
    {
        max7 = fabs(p2_0_p0_0);
    } 
    if( (max7 < fabs(p2_2_p0_2)) )
    {
        max7 = fabs(p2_2_p0_2);
    } 
    if( (max7 < fabs(p2_1_p0_1)) )
    {
        max7 = fabs(p2_1_p0_1);
    } 
    if( (max7 < fabs(p3_0_p0_0)) )
    {
        max7 = fabs(p3_0_p0_0);
    } 
    if( (max7 < fabs(p3_1_p0_1)) )
    {
        max7 = fabs(p3_1_p0_1);
    } 
    if( (max7 < fabs(p3_2_p0_2)) )
    {
        max7 = fabs(p3_2_p0_2);
    } 
    if( (max7 < fabs(p4_0_p0_0)) )
    {
        max7 = fabs(p4_0_p0_0);
    } 
    if( (max7 < fabs(p4_1_p0_1)) )
    {
        max7 = fabs(p4_1_p0_1);
    } 
    if( (max7 < fabs(p4_2_p0_2)) )
    {
        max7 = fabs(p4_2_p0_2);
    } 
    lower_bound_1 = max7;
    upper_bound_1 = max7;
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (lower_bound_1 < 1.12285198342304832993e-59) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 3.21387608851797948065e+60) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.24661365310273025710e-13 * ((((max5 * max6) * max4) * max7) * max7));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta4_sign * int_tmp_result_FFWKCAA);
} 


inline int side4_4d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4, const double* q0, const double* q1, const double* q2, const double* q3) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double l1;
    l1 = (1 * ((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double l2;
    l2 = (1 * ((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double l3;
    l3 = (1 * ((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)));
    double p4_0_p0_0 = (p4[0] - p0[0]);
    double p4_1_p0_1 = (p4[1] - p0[1]);
    double p4_2_p0_2 = (p4[2] - p0[2]);
    double p4_3_p0_3 = (p4[3] - p0[3]);
    double l4;
    l4 = (1 * ((((p4_0_p0_0 * p4_0_p0_0) + (p4_1_p0_1 * p4_1_p0_1)) + (p4_2_p0_2 * p4_2_p0_2)) + (p4_3_p0_3 * p4_3_p0_3)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double a10;
    a10 = (2 * ((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double a11;
    a11 = (2 * ((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double a12;
    a12 = (2 * ((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)));
    double q3_0_p0_0 = (q3[0] - p0[0]);
    double q3_1_p0_1 = (q3[1] - p0[1]);
    double q3_2_p0_2 = (q3[2] - p0[2]);
    double q3_3_p0_3 = (q3[3] - p0[3]);
    double a13;
    a13 = (2 * ((((p1_0_p0_0 * q3_0_p0_0) + (p1_1_p0_1 * q3_1_p0_1)) + (p1_2_p0_2 * q3_2_p0_2)) + (p1_3_p0_3 * q3_3_p0_3)));
    double a20;
    a20 = (2 * ((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)));
    double a21;
    a21 = (2 * ((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)));
    double a22;
    a22 = (2 * ((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)));
    double a23;
    a23 = (2 * ((((p2_0_p0_0 * q3_0_p0_0) + (p2_1_p0_1 * q3_1_p0_1)) + (p2_2_p0_2 * q3_2_p0_2)) + (p2_3_p0_3 * q3_3_p0_3)));
    double a30;
    a30 = (2 * ((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)));
    double a31;
    a31 = (2 * ((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)));
    double a32;
    a32 = (2 * ((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)));
    double a33;
    a33 = (2 * ((((p3_0_p0_0 * q3_0_p0_0) + (p3_1_p0_1 * q3_1_p0_1)) + (p3_2_p0_2 * q3_2_p0_2)) + (p3_3_p0_3 * q3_3_p0_3)));
    double a40;
    a40 = (2 * ((((p4_0_p0_0 * q0_0_p0_0) + (p4_1_p0_1 * q0_1_p0_1)) + (p4_2_p0_2 * q0_2_p0_2)) + (p4_3_p0_3 * q0_3_p0_3)));
    double a41;
    a41 = (2 * ((((p4_0_p0_0 * q1_0_p0_0) + (p4_1_p0_1 * q1_1_p0_1)) + (p4_2_p0_2 * q1_2_p0_2)) + (p4_3_p0_3 * q1_3_p0_3)));
    double a42;
    a42 = (2 * ((((p4_0_p0_0 * q2_0_p0_0) + (p4_1_p0_1 * q2_1_p0_1)) + (p4_2_p0_2 * q2_2_p0_2)) + (p4_3_p0_3 * q2_3_p0_3)));
    double a43;
    a43 = (2 * ((((p4_0_p0_0 * q3_0_p0_0) + (p4_1_p0_1 * q3_1_p0_1)) + (p4_2_p0_2 * q3_2_p0_2)) + (p4_3_p0_3 * q3_3_p0_3)));
    double b00;
    b00 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double b01;
    b01 = -((((a22 * a33) - (a23 * a32)) + ((a23 * a31) - (a21 * a33))) + ((a21 * a32) - (a22 * a31)));
    double b02;
    b02 = ((((a12 * a33) - (a13 * a32)) + ((a13 * a31) - (a11 * a33))) + ((a11 * a32) - (a12 * a31)));
    double b03;
    b03 = -((((a12 * a23) - (a13 * a22)) + ((a13 * a21) - (a11 * a23))) + ((a11 * a22) - (a12 * a21)));
    double b10;
    b10 = -(((a10 * ((a22 * a33) - (a23 * a32))) - (a20 * ((a12 * a33) - (a13 * a32)))) + (a30 * ((a12 * a23) - (a13 * a22))));
    double b11;
    b11 = ((((a22 * a33) - (a23 * a32)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a32) - (a22 * a30)));
    double b12;
    b12 = -((((a12 * a33) - (a13 * a32)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a32) - (a12 * a30)));
    double b13;
    b13 = ((((a12 * a23) - (a13 * a22)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a22) - (a12 * a20)));
    double b20;
    b20 = (((a10 * ((a21 * a33) - (a23 * a31))) - (a20 * ((a11 * a33) - (a13 * a31)))) + (a30 * ((a11 * a23) - (a13 * a21))));
    double b21;
    b21 = -((((a21 * a33) - (a23 * a31)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a31) - (a21 * a30)));
    double b22;
    b22 = ((((a11 * a33) - (a13 * a31)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a31) - (a11 * a30)));
    double b23;
    b23 = -((((a11 * a23) - (a13 * a21)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a21) - (a11 * a20)));
    double b30;
    b30 = -(((a10 * ((a21 * a32) - (a22 * a31))) - (a20 * ((a11 * a32) - (a12 * a31)))) + (a30 * ((a11 * a22) - (a12 * a21))));
    double b31;
    b31 = ((((a21 * a32) - (a22 * a31)) + ((a22 * a30) - (a20 * a32))) + ((a20 * a31) - (a21 * a30)));
    double b32;
    b32 = -((((a11 * a32) - (a12 * a31)) + ((a12 * a30) - (a10 * a32))) + ((a10 * a31) - (a11 * a30)));
    double b33;
    b33 = ((((a11 * a22) - (a12 * a21)) + ((a12 * a20) - (a10 * a22))) + ((a10 * a21) - (a11 * a20)));
    double Delta;
    Delta = (((b00 + b10) + b20) + b30);
    double DeltaLambda0;
    DeltaLambda0 = ((((b01 * l1) + (b02 * l2)) + (b03 * l3)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = ((((b11 * l1) + (b12 * l2)) + (b13 * l3)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = ((((b21 * l1) + (b22 * l2)) + (b23 * l3)) + b20);
    double DeltaLambda3;
    DeltaLambda3 = ((((b31 * l1) + (b32 * l2)) + (b33 * l3)) + b30);
    double r;
    r = ((Delta * l4) - ((((a40 * DeltaLambda0) + (a41 * DeltaLambda1)) + (a42 * DeltaLambda2)) + (a43 * DeltaLambda3)));
    double eps;
    double max1 = fabs(p1_3_p0_3);
    if( (max1 < fabs(p1_0_p0_0)) )
    {
        max1 = fabs(p1_0_p0_0);
    } 
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    double max2 = fabs(p2_2_p0_2);
    if( (max2 < fabs(p2_1_p0_1)) )
    {
        max2 = fabs(p2_1_p0_1);
    } 
    if( (max2 < fabs(p2_3_p0_3)) )
    {
        max2 = fabs(p2_3_p0_3);
    } 
    if( (max2 < fabs(p2_0_p0_0)) )
    {
        max2 = fabs(p2_0_p0_0);
    } 
    double max3 = fabs(p3_0_p0_0);
    if( (max3 < fabs(p3_1_p0_1)) )
    {
        max3 = fabs(p3_1_p0_1);
    } 
    if( (max3 < fabs(p3_3_p0_3)) )
    {
        max3 = fabs(p3_3_p0_3);
    } 
    if( (max3 < fabs(p3_2_p0_2)) )
    {
        max3 = fabs(p3_2_p0_2);
    } 
    double max4 = fabs(q0_3_p0_3);
    if( (max4 < fabs(q0_0_p0_0)) )
    {
        max4 = fabs(q0_0_p0_0);
    } 
    if( (max4 < fabs(q0_1_p0_1)) )
    {
        max4 = fabs(q0_1_p0_1);
    } 
    if( (max4 < fabs(q0_2_p0_2)) )
    {
        max4 = fabs(q0_2_p0_2);
    } 
    if( (max4 < fabs(q1_0_p0_0)) )
    {
        max4 = fabs(q1_0_p0_0);
    } 
    if( (max4 < fabs(q1_1_p0_1)) )
    {
        max4 = fabs(q1_1_p0_1);
    } 
    if( (max4 < fabs(q1_2_p0_2)) )
    {
        max4 = fabs(q1_2_p0_2);
    } 
    if( (max4 < fabs(q1_3_p0_3)) )
    {
        max4 = fabs(q1_3_p0_3);
    } 
    double max5 = fabs(q1_0_p0_0);
    if( (max5 < fabs(q1_1_p0_1)) )
    {
        max5 = fabs(q1_1_p0_1);
    } 
    if( (max5 < fabs(q1_2_p0_2)) )
    {
        max5 = fabs(q1_2_p0_2);
    } 
    if( (max5 < fabs(q1_3_p0_3)) )
    {
        max5 = fabs(q1_3_p0_3);
    } 
    if( (max5 < fabs(q2_0_p0_0)) )
    {
        max5 = fabs(q2_0_p0_0);
    } 
    if( (max5 < fabs(q2_1_p0_1)) )
    {
        max5 = fabs(q2_1_p0_1);
    } 
    if( (max5 < fabs(q2_2_p0_2)) )
    {
        max5 = fabs(q2_2_p0_2);
    } 
    if( (max5 < fabs(q2_3_p0_3)) )
    {
        max5 = fabs(q2_3_p0_3);
    } 
    double max6 = fabs(q2_0_p0_0);
    if( (max6 < fabs(q2_1_p0_1)) )
    {
        max6 = fabs(q2_1_p0_1);
    } 
    if( (max6 < fabs(q2_2_p0_2)) )
    {
        max6 = fabs(q2_2_p0_2);
    } 
    if( (max6 < fabs(q2_3_p0_3)) )
    {
        max6 = fabs(q2_3_p0_3);
    } 
    if( (max6 < fabs(q3_0_p0_0)) )
    {
        max6 = fabs(q3_0_p0_0);
    } 
    if( (max6 < fabs(q3_1_p0_1)) )
    {
        max6 = fabs(q3_1_p0_1);
    } 
    if( (max6 < fabs(q3_2_p0_2)) )
    {
        max6 = fabs(q3_2_p0_2);
    } 
    if( (max6 < fabs(q3_3_p0_3)) )
    {
        max6 = fabs(q3_3_p0_3);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (lower_bound_1 < 4.14607644401726239868e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.38046888801178809320e-12 * (((((max1 * max4) * max2) * max5) * max3) * max6));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max7 = max1;
    if( (max7 < max2) )
    {
        max7 = max2;
    } 
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    if( (max7 < max6) )
    {
        max7 = max6;
    } 
    double max8 = max1;
    if( (max8 < fabs(p4_1_p0_1)) )
    {
        max8 = fabs(p4_1_p0_1);
    } 
    if( (max8 < fabs(p4_2_p0_2)) )
    {
        max8 = fabs(p4_2_p0_2);
    } 
    if( (max8 < fabs(p4_0_p0_0)) )
    {
        max8 = fabs(p4_0_p0_0);
    } 
    if( (max8 < fabs(p4_3_p0_3)) )
    {
        max8 = fabs(p4_3_p0_3);
    } 
    if( (max7 < max8) )
    {
        max7 = max8;
    } 
    double max9 = max1;
    if( (max9 < max5) )
    {
        max9 = max5;
    } 
    if( (max9 < max8) )
    {
        max9 = max8;
    } 
    double max10;
    double max11 = max4;
    if( (max11 < max5) )
    {
        max11 = max5;
    } 
    max10 = max11;
    if( (max10 < max1) )
    {
        max10 = max1;
    } 
    if( (max10 < max4) )
    {
        max10 = max4;
    } 
    if( (max10 < max5) )
    {
        max10 = max5;
    } 
    if( (max10 < max6) )
    {
        max10 = max6;
    } 
    lower_bound_1 = max10;
    upper_bound_1 = max10;
    if( (max11 < lower_bound_1) )
    {
        lower_bound_1 = max11;
    } 
    else 
    {
        if( (max11 > upper_bound_1) )
        {
            upper_bound_1 = max11;
        } 
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (max8 < lower_bound_1) )
    {
        lower_bound_1 = max8;
    } 
    if( (max9 < lower_bound_1) )
    {
        lower_bound_1 = max9;
    } 
    else 
    {
        if( (max9 > upper_bound_1) )
        {
            upper_bound_1 = max9;
        } 
    } 
    if( (lower_bound_1 < 6.06263132863556750071e-38) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.21914442286604163181e-10 * (((((((max8 * max11) * max2) * max10) * max3) * max10) * max9) * max7));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta_sign * int_tmp_result_FFWKCAA);
} 


inline int side4_6d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4, const double* q0, const double* q1, const double* q2, const double* q3) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double l1;
    l1 = (1 * ((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double l2;
    l2 = (1 * ((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double l3;
    l3 = (1 * ((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)));
    double p4_0_p0_0 = (p4[0] - p0[0]);
    double p4_1_p0_1 = (p4[1] - p0[1]);
    double p4_2_p0_2 = (p4[2] - p0[2]);
    double p4_3_p0_3 = (p4[3] - p0[3]);
    double p4_4_p0_4 = (p4[4] - p0[4]);
    double p4_5_p0_5 = (p4[5] - p0[5]);
    double l4;
    l4 = (1 * ((((((p4_0_p0_0 * p4_0_p0_0) + (p4_1_p0_1 * p4_1_p0_1)) + (p4_2_p0_2 * p4_2_p0_2)) + (p4_3_p0_3 * p4_3_p0_3)) + (p4_4_p0_4 * p4_4_p0_4)) + (p4_5_p0_5 * p4_5_p0_5)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double a10;
    a10 = (2 * ((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double a11;
    a11 = (2 * ((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double a12;
    a12 = (2 * ((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)));
    double q3_0_p0_0 = (q3[0] - p0[0]);
    double q3_1_p0_1 = (q3[1] - p0[1]);
    double q3_2_p0_2 = (q3[2] - p0[2]);
    double q3_3_p0_3 = (q3[3] - p0[3]);
    double q3_4_p0_4 = (q3[4] - p0[4]);
    double q3_5_p0_5 = (q3[5] - p0[5]);
    double a13;
    a13 = (2 * ((((((p1_0_p0_0 * q3_0_p0_0) + (p1_1_p0_1 * q3_1_p0_1)) + (p1_2_p0_2 * q3_2_p0_2)) + (p1_3_p0_3 * q3_3_p0_3)) + (p1_4_p0_4 * q3_4_p0_4)) + (p1_5_p0_5 * q3_5_p0_5)));
    double a20;
    a20 = (2 * ((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)));
    double a21;
    a21 = (2 * ((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)));
    double a22;
    a22 = (2 * ((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)));
    double a23;
    a23 = (2 * ((((((p2_0_p0_0 * q3_0_p0_0) + (p2_1_p0_1 * q3_1_p0_1)) + (p2_2_p0_2 * q3_2_p0_2)) + (p2_3_p0_3 * q3_3_p0_3)) + (p2_4_p0_4 * q3_4_p0_4)) + (p2_5_p0_5 * q3_5_p0_5)));
    double a30;
    a30 = (2 * ((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)));
    double a31;
    a31 = (2 * ((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)));
    double a32;
    a32 = (2 * ((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)));
    double a33;
    a33 = (2 * ((((((p3_0_p0_0 * q3_0_p0_0) + (p3_1_p0_1 * q3_1_p0_1)) + (p3_2_p0_2 * q3_2_p0_2)) + (p3_3_p0_3 * q3_3_p0_3)) + (p3_4_p0_4 * q3_4_p0_4)) + (p3_5_p0_5 * q3_5_p0_5)));
    double a40;
    a40 = (2 * ((((((p4_0_p0_0 * q0_0_p0_0) + (p4_1_p0_1 * q0_1_p0_1)) + (p4_2_p0_2 * q0_2_p0_2)) + (p4_3_p0_3 * q0_3_p0_3)) + (p4_4_p0_4 * q0_4_p0_4)) + (p4_5_p0_5 * q0_5_p0_5)));
    double a41;
    a41 = (2 * ((((((p4_0_p0_0 * q1_0_p0_0) + (p4_1_p0_1 * q1_1_p0_1)) + (p4_2_p0_2 * q1_2_p0_2)) + (p4_3_p0_3 * q1_3_p0_3)) + (p4_4_p0_4 * q1_4_p0_4)) + (p4_5_p0_5 * q1_5_p0_5)));
    double a42;
    a42 = (2 * ((((((p4_0_p0_0 * q2_0_p0_0) + (p4_1_p0_1 * q2_1_p0_1)) + (p4_2_p0_2 * q2_2_p0_2)) + (p4_3_p0_3 * q2_3_p0_3)) + (p4_4_p0_4 * q2_4_p0_4)) + (p4_5_p0_5 * q2_5_p0_5)));
    double a43;
    a43 = (2 * ((((((p4_0_p0_0 * q3_0_p0_0) + (p4_1_p0_1 * q3_1_p0_1)) + (p4_2_p0_2 * q3_2_p0_2)) + (p4_3_p0_3 * q3_3_p0_3)) + (p4_4_p0_4 * q3_4_p0_4)) + (p4_5_p0_5 * q3_5_p0_5)));
    double b00;
    b00 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double b01;
    b01 = -((((a22 * a33) - (a23 * a32)) + ((a23 * a31) - (a21 * a33))) + ((a21 * a32) - (a22 * a31)));
    double b02;
    b02 = ((((a12 * a33) - (a13 * a32)) + ((a13 * a31) - (a11 * a33))) + ((a11 * a32) - (a12 * a31)));
    double b03;
    b03 = -((((a12 * a23) - (a13 * a22)) + ((a13 * a21) - (a11 * a23))) + ((a11 * a22) - (a12 * a21)));
    double b10;
    b10 = -(((a10 * ((a22 * a33) - (a23 * a32))) - (a20 * ((a12 * a33) - (a13 * a32)))) + (a30 * ((a12 * a23) - (a13 * a22))));
    double b11;
    b11 = ((((a22 * a33) - (a23 * a32)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a32) - (a22 * a30)));
    double b12;
    b12 = -((((a12 * a33) - (a13 * a32)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a32) - (a12 * a30)));
    double b13;
    b13 = ((((a12 * a23) - (a13 * a22)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a22) - (a12 * a20)));
    double b20;
    b20 = (((a10 * ((a21 * a33) - (a23 * a31))) - (a20 * ((a11 * a33) - (a13 * a31)))) + (a30 * ((a11 * a23) - (a13 * a21))));
    double b21;
    b21 = -((((a21 * a33) - (a23 * a31)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a31) - (a21 * a30)));
    double b22;
    b22 = ((((a11 * a33) - (a13 * a31)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a31) - (a11 * a30)));
    double b23;
    b23 = -((((a11 * a23) - (a13 * a21)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a21) - (a11 * a20)));
    double b30;
    b30 = -(((a10 * ((a21 * a32) - (a22 * a31))) - (a20 * ((a11 * a32) - (a12 * a31)))) + (a30 * ((a11 * a22) - (a12 * a21))));
    double b31;
    b31 = ((((a21 * a32) - (a22 * a31)) + ((a22 * a30) - (a20 * a32))) + ((a20 * a31) - (a21 * a30)));
    double b32;
    b32 = -((((a11 * a32) - (a12 * a31)) + ((a12 * a30) - (a10 * a32))) + ((a10 * a31) - (a11 * a30)));
    double b33;
    b33 = ((((a11 * a22) - (a12 * a21)) + ((a12 * a20) - (a10 * a22))) + ((a10 * a21) - (a11 * a20)));
    double Delta;
    Delta = (((b00 + b10) + b20) + b30);
    double DeltaLambda0;
    DeltaLambda0 = ((((b01 * l1) + (b02 * l2)) + (b03 * l3)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = ((((b11 * l1) + (b12 * l2)) + (b13 * l3)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = ((((b21 * l1) + (b22 * l2)) + (b23 * l3)) + b20);
    double DeltaLambda3;
    DeltaLambda3 = ((((b31 * l1) + (b32 * l2)) + (b33 * l3)) + b30);
    double r;
    r = ((Delta * l4) - ((((a40 * DeltaLambda0) + (a41 * DeltaLambda1)) + (a42 * DeltaLambda2)) + (a43 * DeltaLambda3)));
    double eps;
    double max1 = fabs(p3_2_p0_2);
    if( (max1 < fabs(p3_0_p0_0)) )
    {
        max1 = fabs(p3_0_p0_0);
    } 
    if( (max1 < fabs(p3_3_p0_3)) )
    {
        max1 = fabs(p3_3_p0_3);
    } 
    if( (max1 < fabs(p3_4_p0_4)) )
    {
        max1 = fabs(p3_4_p0_4);
    } 
    if( (max1 < fabs(p3_1_p0_1)) )
    {
        max1 = fabs(p3_1_p0_1);
    } 
    if( (max1 < fabs(p3_5_p0_5)) )
    {
        max1 = fabs(p3_5_p0_5);
    } 
    double max2 = fabs(p2_1_p0_1);
    if( (max2 < fabs(p2_4_p0_4)) )
    {
        max2 = fabs(p2_4_p0_4);
    } 
    if( (max2 < fabs(p2_2_p0_2)) )
    {
        max2 = fabs(p2_2_p0_2);
    } 
    if( (max2 < fabs(p2_0_p0_0)) )
    {
        max2 = fabs(p2_0_p0_0);
    } 
    if( (max2 < fabs(p2_3_p0_3)) )
    {
        max2 = fabs(p2_3_p0_3);
    } 
    if( (max2 < fabs(p2_5_p0_5)) )
    {
        max2 = fabs(p2_5_p0_5);
    } 
    double max3 = fabs(p1_0_p0_0);
    if( (max3 < fabs(p1_1_p0_1)) )
    {
        max3 = fabs(p1_1_p0_1);
    } 
    if( (max3 < fabs(p1_2_p0_2)) )
    {
        max3 = fabs(p1_2_p0_2);
    } 
    if( (max3 < fabs(p1_3_p0_3)) )
    {
        max3 = fabs(p1_3_p0_3);
    } 
    if( (max3 < fabs(p1_4_p0_4)) )
    {
        max3 = fabs(p1_4_p0_4);
    } 
    if( (max3 < fabs(p1_5_p0_5)) )
    {
        max3 = fabs(p1_5_p0_5);
    } 
    double max4 = fabs(q0_0_p0_0);
    if( (max4 < fabs(q0_1_p0_1)) )
    {
        max4 = fabs(q0_1_p0_1);
    } 
    if( (max4 < fabs(q0_2_p0_2)) )
    {
        max4 = fabs(q0_2_p0_2);
    } 
    if( (max4 < fabs(q0_3_p0_3)) )
    {
        max4 = fabs(q0_3_p0_3);
    } 
    if( (max4 < fabs(q0_4_p0_4)) )
    {
        max4 = fabs(q0_4_p0_4);
    } 
    if( (max4 < fabs(q0_5_p0_5)) )
    {
        max4 = fabs(q0_5_p0_5);
    } 
    if( (max4 < fabs(q1_0_p0_0)) )
    {
        max4 = fabs(q1_0_p0_0);
    } 
    if( (max4 < fabs(q1_1_p0_1)) )
    {
        max4 = fabs(q1_1_p0_1);
    } 
    if( (max4 < fabs(q1_2_p0_2)) )
    {
        max4 = fabs(q1_2_p0_2);
    } 
    if( (max4 < fabs(q1_3_p0_3)) )
    {
        max4 = fabs(q1_3_p0_3);
    } 
    if( (max4 < fabs(q1_4_p0_4)) )
    {
        max4 = fabs(q1_4_p0_4);
    } 
    if( (max4 < fabs(q1_5_p0_5)) )
    {
        max4 = fabs(q1_5_p0_5);
    } 
    double max5 = fabs(q1_0_p0_0);
    if( (max5 < fabs(q1_1_p0_1)) )
    {
        max5 = fabs(q1_1_p0_1);
    } 
    if( (max5 < fabs(q1_2_p0_2)) )
    {
        max5 = fabs(q1_2_p0_2);
    } 
    if( (max5 < fabs(q1_3_p0_3)) )
    {
        max5 = fabs(q1_3_p0_3);
    } 
    if( (max5 < fabs(q1_4_p0_4)) )
    {
        max5 = fabs(q1_4_p0_4);
    } 
    if( (max5 < fabs(q1_5_p0_5)) )
    {
        max5 = fabs(q1_5_p0_5);
    } 
    if( (max5 < fabs(q2_0_p0_0)) )
    {
        max5 = fabs(q2_0_p0_0);
    } 
    if( (max5 < fabs(q2_1_p0_1)) )
    {
        max5 = fabs(q2_1_p0_1);
    } 
    if( (max5 < fabs(q2_2_p0_2)) )
    {
        max5 = fabs(q2_2_p0_2);
    } 
    if( (max5 < fabs(q2_3_p0_3)) )
    {
        max5 = fabs(q2_3_p0_3);
    } 
    if( (max5 < fabs(q2_4_p0_4)) )
    {
        max5 = fabs(q2_4_p0_4);
    } 
    if( (max5 < fabs(q2_5_p0_5)) )
    {
        max5 = fabs(q2_5_p0_5);
    } 
    double max6 = fabs(q2_0_p0_0);
    if( (max6 < fabs(q2_1_p0_1)) )
    {
        max6 = fabs(q2_1_p0_1);
    } 
    if( (max6 < fabs(q2_2_p0_2)) )
    {
        max6 = fabs(q2_2_p0_2);
    } 
    if( (max6 < fabs(q2_3_p0_3)) )
    {
        max6 = fabs(q2_3_p0_3);
    } 
    if( (max6 < fabs(q2_4_p0_4)) )
    {
        max6 = fabs(q2_4_p0_4);
    } 
    if( (max6 < fabs(q2_5_p0_5)) )
    {
        max6 = fabs(q2_5_p0_5);
    } 
    if( (max6 < fabs(q3_0_p0_0)) )
    {
        max6 = fabs(q3_0_p0_0);
    } 
    if( (max6 < fabs(q3_1_p0_1)) )
    {
        max6 = fabs(q3_1_p0_1);
    } 
    if( (max6 < fabs(q3_2_p0_2)) )
    {
        max6 = fabs(q3_2_p0_2);
    } 
    if( (max6 < fabs(q3_3_p0_3)) )
    {
        max6 = fabs(q3_3_p0_3);
    } 
    if( (max6 < fabs(q3_4_p0_4)) )
    {
        max6 = fabs(q3_4_p0_4);
    } 
    if( (max6 < fabs(q3_5_p0_5)) )
    {
        max6 = fabs(q3_5_p0_5);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max4;
    upper_bound_1 = max4;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (lower_bound_1 < 3.31864264949884013629e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.66564133587113165316e-11 * (((((max3 * max4) * max2) * max5) * max1) * max6));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max7 = max1;
    if( (max7 < max2) )
    {
        max7 = max2;
    } 
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    double max8 = max3;
    if( (max8 < fabs(p4_5_p0_5)) )
    {
        max8 = fabs(p4_5_p0_5);
    } 
    if( (max8 < fabs(p4_0_p0_0)) )
    {
        max8 = fabs(p4_0_p0_0);
    } 
    if( (max8 < fabs(p4_1_p0_1)) )
    {
        max8 = fabs(p4_1_p0_1);
    } 
    if( (max8 < fabs(p4_2_p0_2)) )
    {
        max8 = fabs(p4_2_p0_2);
    } 
    if( (max8 < fabs(p4_3_p0_3)) )
    {
        max8 = fabs(p4_3_p0_3);
    } 
    if( (max8 < fabs(p4_4_p0_4)) )
    {
        max8 = fabs(p4_4_p0_4);
    } 
    if( (max7 < max8) )
    {
        max7 = max8;
    } 
    if( (max7 < max6) )
    {
        max7 = max6;
    } 
    double max9 = max3;
    if( (max9 < max8) )
    {
        max9 = max8;
    } 
    if( (max9 < max5) )
    {
        max9 = max5;
    } 
    double max10 = max4;
    if( (max10 < max3) )
    {
        max10 = max3;
    } 
    if( (max10 < max5) )
    {
        max10 = max5;
    } 
    double max11 = max4;
    if( (max11 < max5) )
    {
        max11 = max5;
    } 
    if( (max10 < max11) )
    {
        max10 = max11;
    } 
    if( (max10 < max6) )
    {
        max10 = max6;
    } 
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (max8 < lower_bound_1) )
    {
        lower_bound_1 = max8;
    } 
    if( (max9 < lower_bound_1) )
    {
        lower_bound_1 = max9;
    } 
    else 
    {
        if( (max9 > upper_bound_1) )
        {
            upper_bound_1 = max9;
        } 
    } 
    if( (max10 < lower_bound_1) )
    {
        lower_bound_1 = max10;
    } 
    else 
    {
        if( (max10 > upper_bound_1) )
        {
            upper_bound_1 = max10;
        } 
    } 
    if( (max11 < lower_bound_1) )
    {
        lower_bound_1 = max11;
    } 
    else 
    {
        if( (max11 > upper_bound_1) )
        {
            upper_bound_1 = max11;
        } 
    } 
    if( (lower_bound_1 < 4.87975611107819181771e-38) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (6.92085098542795335117e-10 * (((((((max8 * max11) * max2) * max10) * max1) * max10) * max9) * max7));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta_sign * int_tmp_result_FFWKCAA);
} 


inline int side4_7d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4, const double* q0, const double* q1, const double* q2, const double* q3) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double l1;
    l1 = (1 * (((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double l2;
    l2 = (1 * (((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double p3_6_p0_6 = (p3[6] - p0[6]);
    double l3;
    l3 = (1 * (((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)) + (p3_6_p0_6 * p3_6_p0_6)));
    double p4_0_p0_0 = (p4[0] - p0[0]);
    double p4_1_p0_1 = (p4[1] - p0[1]);
    double p4_2_p0_2 = (p4[2] - p0[2]);
    double p4_3_p0_3 = (p4[3] - p0[3]);
    double p4_4_p0_4 = (p4[4] - p0[4]);
    double p4_5_p0_5 = (p4[5] - p0[5]);
    double p4_6_p0_6 = (p4[6] - p0[6]);
    double l4;
    l4 = (1 * (((((((p4_0_p0_0 * p4_0_p0_0) + (p4_1_p0_1 * p4_1_p0_1)) + (p4_2_p0_2 * p4_2_p0_2)) + (p4_3_p0_3 * p4_3_p0_3)) + (p4_4_p0_4 * p4_4_p0_4)) + (p4_5_p0_5 * p4_5_p0_5)) + (p4_6_p0_6 * p4_6_p0_6)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double a10;
    a10 = (2 * (((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double a11;
    a11 = (2 * (((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double q2_6_p0_6 = (q2[6] - p0[6]);
    double a12;
    a12 = (2 * (((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)) + (p1_6_p0_6 * q2_6_p0_6)));
    double q3_0_p0_0 = (q3[0] - p0[0]);
    double q3_1_p0_1 = (q3[1] - p0[1]);
    double q3_2_p0_2 = (q3[2] - p0[2]);
    double q3_3_p0_3 = (q3[3] - p0[3]);
    double q3_4_p0_4 = (q3[4] - p0[4]);
    double q3_5_p0_5 = (q3[5] - p0[5]);
    double q3_6_p0_6 = (q3[6] - p0[6]);
    double a13;
    a13 = (2 * (((((((p1_0_p0_0 * q3_0_p0_0) + (p1_1_p0_1 * q3_1_p0_1)) + (p1_2_p0_2 * q3_2_p0_2)) + (p1_3_p0_3 * q3_3_p0_3)) + (p1_4_p0_4 * q3_4_p0_4)) + (p1_5_p0_5 * q3_5_p0_5)) + (p1_6_p0_6 * q3_6_p0_6)));
    double a20;
    a20 = (2 * (((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)));
    double a21;
    a21 = (2 * (((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)));
    double a22;
    a22 = (2 * (((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)) + (p2_6_p0_6 * q2_6_p0_6)));
    double a23;
    a23 = (2 * (((((((p2_0_p0_0 * q3_0_p0_0) + (p2_1_p0_1 * q3_1_p0_1)) + (p2_2_p0_2 * q3_2_p0_2)) + (p2_3_p0_3 * q3_3_p0_3)) + (p2_4_p0_4 * q3_4_p0_4)) + (p2_5_p0_5 * q3_5_p0_5)) + (p2_6_p0_6 * q3_6_p0_6)));
    double a30;
    a30 = (2 * (((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)) + (p3_6_p0_6 * q0_6_p0_6)));
    double a31;
    a31 = (2 * (((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)) + (p3_6_p0_6 * q1_6_p0_6)));
    double a32;
    a32 = (2 * (((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)) + (p3_6_p0_6 * q2_6_p0_6)));
    double a33;
    a33 = (2 * (((((((p3_0_p0_0 * q3_0_p0_0) + (p3_1_p0_1 * q3_1_p0_1)) + (p3_2_p0_2 * q3_2_p0_2)) + (p3_3_p0_3 * q3_3_p0_3)) + (p3_4_p0_4 * q3_4_p0_4)) + (p3_5_p0_5 * q3_5_p0_5)) + (p3_6_p0_6 * q3_6_p0_6)));
    double a40;
    a40 = (2 * (((((((p4_0_p0_0 * q0_0_p0_0) + (p4_1_p0_1 * q0_1_p0_1)) + (p4_2_p0_2 * q0_2_p0_2)) + (p4_3_p0_3 * q0_3_p0_3)) + (p4_4_p0_4 * q0_4_p0_4)) + (p4_5_p0_5 * q0_5_p0_5)) + (p4_6_p0_6 * q0_6_p0_6)));
    double a41;
    a41 = (2 * (((((((p4_0_p0_0 * q1_0_p0_0) + (p4_1_p0_1 * q1_1_p0_1)) + (p4_2_p0_2 * q1_2_p0_2)) + (p4_3_p0_3 * q1_3_p0_3)) + (p4_4_p0_4 * q1_4_p0_4)) + (p4_5_p0_5 * q1_5_p0_5)) + (p4_6_p0_6 * q1_6_p0_6)));
    double a42;
    a42 = (2 * (((((((p4_0_p0_0 * q2_0_p0_0) + (p4_1_p0_1 * q2_1_p0_1)) + (p4_2_p0_2 * q2_2_p0_2)) + (p4_3_p0_3 * q2_3_p0_3)) + (p4_4_p0_4 * q2_4_p0_4)) + (p4_5_p0_5 * q2_5_p0_5)) + (p4_6_p0_6 * q2_6_p0_6)));
    double a43;
    a43 = (2 * (((((((p4_0_p0_0 * q3_0_p0_0) + (p4_1_p0_1 * q3_1_p0_1)) + (p4_2_p0_2 * q3_2_p0_2)) + (p4_3_p0_3 * q3_3_p0_3)) + (p4_4_p0_4 * q3_4_p0_4)) + (p4_5_p0_5 * q3_5_p0_5)) + (p4_6_p0_6 * q3_6_p0_6)));
    double b00;
    b00 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double b01;
    b01 = -((((a22 * a33) - (a23 * a32)) + ((a23 * a31) - (a21 * a33))) + ((a21 * a32) - (a22 * a31)));
    double b02;
    b02 = ((((a12 * a33) - (a13 * a32)) + ((a13 * a31) - (a11 * a33))) + ((a11 * a32) - (a12 * a31)));
    double b03;
    b03 = -((((a12 * a23) - (a13 * a22)) + ((a13 * a21) - (a11 * a23))) + ((a11 * a22) - (a12 * a21)));
    double b10;
    b10 = -(((a10 * ((a22 * a33) - (a23 * a32))) - (a20 * ((a12 * a33) - (a13 * a32)))) + (a30 * ((a12 * a23) - (a13 * a22))));
    double b11;
    b11 = ((((a22 * a33) - (a23 * a32)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a32) - (a22 * a30)));
    double b12;
    b12 = -((((a12 * a33) - (a13 * a32)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a32) - (a12 * a30)));
    double b13;
    b13 = ((((a12 * a23) - (a13 * a22)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a22) - (a12 * a20)));
    double b20;
    b20 = (((a10 * ((a21 * a33) - (a23 * a31))) - (a20 * ((a11 * a33) - (a13 * a31)))) + (a30 * ((a11 * a23) - (a13 * a21))));
    double b21;
    b21 = -((((a21 * a33) - (a23 * a31)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a31) - (a21 * a30)));
    double b22;
    b22 = ((((a11 * a33) - (a13 * a31)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a31) - (a11 * a30)));
    double b23;
    b23 = -((((a11 * a23) - (a13 * a21)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a21) - (a11 * a20)));
    double b30;
    b30 = -(((a10 * ((a21 * a32) - (a22 * a31))) - (a20 * ((a11 * a32) - (a12 * a31)))) + (a30 * ((a11 * a22) - (a12 * a21))));
    double b31;
    b31 = ((((a21 * a32) - (a22 * a31)) + ((a22 * a30) - (a20 * a32))) + ((a20 * a31) - (a21 * a30)));
    double b32;
    b32 = -((((a11 * a32) - (a12 * a31)) + ((a12 * a30) - (a10 * a32))) + ((a10 * a31) - (a11 * a30)));
    double b33;
    b33 = ((((a11 * a22) - (a12 * a21)) + ((a12 * a20) - (a10 * a22))) + ((a10 * a21) - (a11 * a20)));
    double Delta;
    Delta = (((b00 + b10) + b20) + b30);
    double DeltaLambda0;
    DeltaLambda0 = ((((b01 * l1) + (b02 * l2)) + (b03 * l3)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = ((((b11 * l1) + (b12 * l2)) + (b13 * l3)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = ((((b21 * l1) + (b22 * l2)) + (b23 * l3)) + b20);
    double DeltaLambda3;
    DeltaLambda3 = ((((b31 * l1) + (b32 * l2)) + (b33 * l3)) + b30);
    double r;
    r = ((Delta * l4) - ((((a40 * DeltaLambda0) + (a41 * DeltaLambda1)) + (a42 * DeltaLambda2)) + (a43 * DeltaLambda3)));
    double eps;
    double max1 = fabs(p1_0_p0_0);
    if( (max1 < fabs(p1_1_p0_1)) )
    {
        max1 = fabs(p1_1_p0_1);
    } 
    if( (max1 < fabs(p1_2_p0_2)) )
    {
        max1 = fabs(p1_2_p0_2);
    } 
    if( (max1 < fabs(p1_3_p0_3)) )
    {
        max1 = fabs(p1_3_p0_3);
    } 
    if( (max1 < fabs(p1_4_p0_4)) )
    {
        max1 = fabs(p1_4_p0_4);
    } 
    if( (max1 < fabs(p1_5_p0_5)) )
    {
        max1 = fabs(p1_5_p0_5);
    } 
    if( (max1 < fabs(p1_6_p0_6)) )
    {
        max1 = fabs(p1_6_p0_6);
    } 
    double max2 = fabs(p3_0_p0_0);
    if( (max2 < fabs(p3_4_p0_4)) )
    {
        max2 = fabs(p3_4_p0_4);
    } 
    if( (max2 < fabs(p3_2_p0_2)) )
    {
        max2 = fabs(p3_2_p0_2);
    } 
    if( (max2 < fabs(p3_1_p0_1)) )
    {
        max2 = fabs(p3_1_p0_1);
    } 
    if( (max2 < fabs(p3_3_p0_3)) )
    {
        max2 = fabs(p3_3_p0_3);
    } 
    if( (max2 < fabs(p3_5_p0_5)) )
    {
        max2 = fabs(p3_5_p0_5);
    } 
    if( (max2 < fabs(p3_6_p0_6)) )
    {
        max2 = fabs(p3_6_p0_6);
    } 
    double max3 = fabs(p2_5_p0_5);
    if( (max3 < fabs(p2_2_p0_2)) )
    {
        max3 = fabs(p2_2_p0_2);
    } 
    if( (max3 < fabs(p2_3_p0_3)) )
    {
        max3 = fabs(p2_3_p0_3);
    } 
    if( (max3 < fabs(p2_0_p0_0)) )
    {
        max3 = fabs(p2_0_p0_0);
    } 
    if( (max3 < fabs(p2_1_p0_1)) )
    {
        max3 = fabs(p2_1_p0_1);
    } 
    if( (max3 < fabs(p2_6_p0_6)) )
    {
        max3 = fabs(p2_6_p0_6);
    } 
    if( (max3 < fabs(p2_4_p0_4)) )
    {
        max3 = fabs(p2_4_p0_4);
    } 
    double max4 = fabs(q0_0_p0_0);
    if( (max4 < fabs(q0_1_p0_1)) )
    {
        max4 = fabs(q0_1_p0_1);
    } 
    if( (max4 < fabs(q0_2_p0_2)) )
    {
        max4 = fabs(q0_2_p0_2);
    } 
    if( (max4 < fabs(q0_3_p0_3)) )
    {
        max4 = fabs(q0_3_p0_3);
    } 
    if( (max4 < fabs(q0_4_p0_4)) )
    {
        max4 = fabs(q0_4_p0_4);
    } 
    if( (max4 < fabs(q0_5_p0_5)) )
    {
        max4 = fabs(q0_5_p0_5);
    } 
    if( (max4 < fabs(q0_6_p0_6)) )
    {
        max4 = fabs(q0_6_p0_6);
    } 
    if( (max4 < fabs(q1_0_p0_0)) )
    {
        max4 = fabs(q1_0_p0_0);
    } 
    if( (max4 < fabs(q1_1_p0_1)) )
    {
        max4 = fabs(q1_1_p0_1);
    } 
    if( (max4 < fabs(q1_2_p0_2)) )
    {
        max4 = fabs(q1_2_p0_2);
    } 
    if( (max4 < fabs(q1_3_p0_3)) )
    {
        max4 = fabs(q1_3_p0_3);
    } 
    if( (max4 < fabs(q1_4_p0_4)) )
    {
        max4 = fabs(q1_4_p0_4);
    } 
    if( (max4 < fabs(q1_5_p0_5)) )
    {
        max4 = fabs(q1_5_p0_5);
    } 
    if( (max4 < fabs(q1_6_p0_6)) )
    {
        max4 = fabs(q1_6_p0_6);
    } 
    double max5 = fabs(q1_0_p0_0);
    if( (max5 < fabs(q1_1_p0_1)) )
    {
        max5 = fabs(q1_1_p0_1);
    } 
    if( (max5 < fabs(q1_2_p0_2)) )
    {
        max5 = fabs(q1_2_p0_2);
    } 
    if( (max5 < fabs(q1_3_p0_3)) )
    {
        max5 = fabs(q1_3_p0_3);
    } 
    if( (max5 < fabs(q1_4_p0_4)) )
    {
        max5 = fabs(q1_4_p0_4);
    } 
    if( (max5 < fabs(q1_5_p0_5)) )
    {
        max5 = fabs(q1_5_p0_5);
    } 
    if( (max5 < fabs(q1_6_p0_6)) )
    {
        max5 = fabs(q1_6_p0_6);
    } 
    if( (max5 < fabs(q2_0_p0_0)) )
    {
        max5 = fabs(q2_0_p0_0);
    } 
    if( (max5 < fabs(q2_1_p0_1)) )
    {
        max5 = fabs(q2_1_p0_1);
    } 
    if( (max5 < fabs(q2_2_p0_2)) )
    {
        max5 = fabs(q2_2_p0_2);
    } 
    if( (max5 < fabs(q2_3_p0_3)) )
    {
        max5 = fabs(q2_3_p0_3);
    } 
    if( (max5 < fabs(q2_4_p0_4)) )
    {
        max5 = fabs(q2_4_p0_4);
    } 
    if( (max5 < fabs(q2_5_p0_5)) )
    {
        max5 = fabs(q2_5_p0_5);
    } 
    if( (max5 < fabs(q2_6_p0_6)) )
    {
        max5 = fabs(q2_6_p0_6);
    } 
    double max6 = fabs(q2_0_p0_0);
    if( (max6 < fabs(q2_1_p0_1)) )
    {
        max6 = fabs(q2_1_p0_1);
    } 
    if( (max6 < fabs(q2_2_p0_2)) )
    {
        max6 = fabs(q2_2_p0_2);
    } 
    if( (max6 < fabs(q2_3_p0_3)) )
    {
        max6 = fabs(q2_3_p0_3);
    } 
    if( (max6 < fabs(q2_4_p0_4)) )
    {
        max6 = fabs(q2_4_p0_4);
    } 
    if( (max6 < fabs(q2_5_p0_5)) )
    {
        max6 = fabs(q2_5_p0_5);
    } 
    if( (max6 < fabs(q2_6_p0_6)) )
    {
        max6 = fabs(q2_6_p0_6);
    } 
    if( (max6 < fabs(q3_0_p0_0)) )
    {
        max6 = fabs(q3_0_p0_0);
    } 
    if( (max6 < fabs(q3_1_p0_1)) )
    {
        max6 = fabs(q3_1_p0_1);
    } 
    if( (max6 < fabs(q3_2_p0_2)) )
    {
        max6 = fabs(q3_2_p0_2);
    } 
    if( (max6 < fabs(q3_3_p0_3)) )
    {
        max6 = fabs(q3_3_p0_3);
    } 
    if( (max6 < fabs(q3_4_p0_4)) )
    {
        max6 = fabs(q3_4_p0_4);
    } 
    if( (max6 < fabs(q3_5_p0_5)) )
    {
        max6 = fabs(q3_5_p0_5);
    } 
    if( (max6 < fabs(q3_6_p0_6)) )
    {
        max6 = fabs(q3_6_p0_6);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max3;
    upper_bound_1 = max3;
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (lower_bound_1 < 3.04548303565602498901e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (2.78873548804336160566e-11 * (((((max1 * max4) * max3) * max5) * max2) * max6));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max7 = max3;
    if( (max7 < max1) )
    {
        max7 = max1;
    } 
    if( (max7 < max2) )
    {
        max7 = max2;
    } 
    if( (max7 < max6) )
    {
        max7 = max6;
    } 
    double max8 = max1;
    if( (max8 < fabs(p4_4_p0_4)) )
    {
        max8 = fabs(p4_4_p0_4);
    } 
    if( (max8 < fabs(p4_1_p0_1)) )
    {
        max8 = fabs(p4_1_p0_1);
    } 
    if( (max8 < fabs(p4_5_p0_5)) )
    {
        max8 = fabs(p4_5_p0_5);
    } 
    if( (max8 < fabs(p4_0_p0_0)) )
    {
        max8 = fabs(p4_0_p0_0);
    } 
    if( (max8 < fabs(p4_2_p0_2)) )
    {
        max8 = fabs(p4_2_p0_2);
    } 
    if( (max8 < fabs(p4_3_p0_3)) )
    {
        max8 = fabs(p4_3_p0_3);
    } 
    if( (max8 < fabs(p4_6_p0_6)) )
    {
        max8 = fabs(p4_6_p0_6);
    } 
    if( (max7 < max8) )
    {
        max7 = max8;
    } 
    double max9 = max1;
    if( (max9 < max5) )
    {
        max9 = max5;
    } 
    if( (max9 < max8) )
    {
        max9 = max8;
    } 
    double max10 = max4;
    if( (max10 < max1) )
    {
        max10 = max1;
    } 
    if( (max10 < max5) )
    {
        max10 = max5;
    } 
    if( (max10 < max6) )
    {
        max10 = max6;
    } 
    double max11 = max4;
    if( (max11 < max5) )
    {
        max11 = max5;
    } 
    if( (max10 < max11) )
    {
        max10 = max11;
    } 
    lower_bound_1 = max3;
    upper_bound_1 = max3;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (max8 < lower_bound_1) )
    {
        lower_bound_1 = max8;
    } 
    if( (max9 < lower_bound_1) )
    {
        lower_bound_1 = max9;
    } 
    else 
    {
        if( (max9 > upper_bound_1) )
        {
            upper_bound_1 = max9;
        } 
    } 
    if( (max10 < lower_bound_1) )
    {
        lower_bound_1 = max10;
    } 
    else 
    {
        if( (max10 > upper_bound_1) )
        {
            upper_bound_1 = max10;
        } 
    } 
    if( (max11 < lower_bound_1) )
    {
        lower_bound_1 = max11;
    } 
    else 
    {
        if( (max11 > upper_bound_1) )
        {
            upper_bound_1 = max11;
        } 
    } 
    if( (lower_bound_1 < 4.48906690519700369396e-38) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.34926049830188433875e-09 * (((((((max8 * max11) * max3) * max10) * max2) * max10) * max9) * max7));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta_sign * int_tmp_result_FFWKCAA);
} 


inline int side4_8d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4, const double* q0, const double* q1, const double* q2, const double* q3) {
    double p1_0_p0_0 = (p1[0] - p0[0]);
    double p1_1_p0_1 = (p1[1] - p0[1]);
    double p1_2_p0_2 = (p1[2] - p0[2]);
    double p1_3_p0_3 = (p1[3] - p0[3]);
    double p1_4_p0_4 = (p1[4] - p0[4]);
    double p1_5_p0_5 = (p1[5] - p0[5]);
    double p1_6_p0_6 = (p1[6] - p0[6]);
    double p1_7_p0_7 = (p1[7] - p0[7]);
    double l1;
    l1 = (1 * ((((((((p1_0_p0_0 * p1_0_p0_0) + (p1_1_p0_1 * p1_1_p0_1)) + (p1_2_p0_2 * p1_2_p0_2)) + (p1_3_p0_3 * p1_3_p0_3)) + (p1_4_p0_4 * p1_4_p0_4)) + (p1_5_p0_5 * p1_5_p0_5)) + (p1_6_p0_6 * p1_6_p0_6)) + (p1_7_p0_7 * p1_7_p0_7)));
    double p2_0_p0_0 = (p2[0] - p0[0]);
    double p2_1_p0_1 = (p2[1] - p0[1]);
    double p2_2_p0_2 = (p2[2] - p0[2]);
    double p2_3_p0_3 = (p2[3] - p0[3]);
    double p2_4_p0_4 = (p2[4] - p0[4]);
    double p2_5_p0_5 = (p2[5] - p0[5]);
    double p2_6_p0_6 = (p2[6] - p0[6]);
    double p2_7_p0_7 = (p2[7] - p0[7]);
    double l2;
    l2 = (1 * ((((((((p2_0_p0_0 * p2_0_p0_0) + (p2_1_p0_1 * p2_1_p0_1)) + (p2_2_p0_2 * p2_2_p0_2)) + (p2_3_p0_3 * p2_3_p0_3)) + (p2_4_p0_4 * p2_4_p0_4)) + (p2_5_p0_5 * p2_5_p0_5)) + (p2_6_p0_6 * p2_6_p0_6)) + (p2_7_p0_7 * p2_7_p0_7)));
    double p3_0_p0_0 = (p3[0] - p0[0]);
    double p3_1_p0_1 = (p3[1] - p0[1]);
    double p3_2_p0_2 = (p3[2] - p0[2]);
    double p3_3_p0_3 = (p3[3] - p0[3]);
    double p3_4_p0_4 = (p3[4] - p0[4]);
    double p3_5_p0_5 = (p3[5] - p0[5]);
    double p3_6_p0_6 = (p3[6] - p0[6]);
    double p3_7_p0_7 = (p3[7] - p0[7]);
    double l3;
    l3 = (1 * ((((((((p3_0_p0_0 * p3_0_p0_0) + (p3_1_p0_1 * p3_1_p0_1)) + (p3_2_p0_2 * p3_2_p0_2)) + (p3_3_p0_3 * p3_3_p0_3)) + (p3_4_p0_4 * p3_4_p0_4)) + (p3_5_p0_5 * p3_5_p0_5)) + (p3_6_p0_6 * p3_6_p0_6)) + (p3_7_p0_7 * p3_7_p0_7)));
    double p4_0_p0_0 = (p4[0] - p0[0]);
    double p4_1_p0_1 = (p4[1] - p0[1]);
    double p4_2_p0_2 = (p4[2] - p0[2]);
    double p4_3_p0_3 = (p4[3] - p0[3]);
    double p4_4_p0_4 = (p4[4] - p0[4]);
    double p4_5_p0_5 = (p4[5] - p0[5]);
    double p4_6_p0_6 = (p4[6] - p0[6]);
    double p4_7_p0_7 = (p4[7] - p0[7]);
    double l4;
    l4 = (1 * ((((((((p4_0_p0_0 * p4_0_p0_0) + (p4_1_p0_1 * p4_1_p0_1)) + (p4_2_p0_2 * p4_2_p0_2)) + (p4_3_p0_3 * p4_3_p0_3)) + (p4_4_p0_4 * p4_4_p0_4)) + (p4_5_p0_5 * p4_5_p0_5)) + (p4_6_p0_6 * p4_6_p0_6)) + (p4_7_p0_7 * p4_7_p0_7)));
    double q0_0_p0_0 = (q0[0] - p0[0]);
    double q0_1_p0_1 = (q0[1] - p0[1]);
    double q0_2_p0_2 = (q0[2] - p0[2]);
    double q0_3_p0_3 = (q0[3] - p0[3]);
    double q0_4_p0_4 = (q0[4] - p0[4]);
    double q0_5_p0_5 = (q0[5] - p0[5]);
    double q0_6_p0_6 = (q0[6] - p0[6]);
    double q0_7_p0_7 = (q0[7] - p0[7]);
    double a10;
    a10 = (2 * ((((((((p1_0_p0_0 * q0_0_p0_0) + (p1_1_p0_1 * q0_1_p0_1)) + (p1_2_p0_2 * q0_2_p0_2)) + (p1_3_p0_3 * q0_3_p0_3)) + (p1_4_p0_4 * q0_4_p0_4)) + (p1_5_p0_5 * q0_5_p0_5)) + (p1_6_p0_6 * q0_6_p0_6)) + (p1_7_p0_7 * q0_7_p0_7)));
    double q1_0_p0_0 = (q1[0] - p0[0]);
    double q1_1_p0_1 = (q1[1] - p0[1]);
    double q1_2_p0_2 = (q1[2] - p0[2]);
    double q1_3_p0_3 = (q1[3] - p0[3]);
    double q1_4_p0_4 = (q1[4] - p0[4]);
    double q1_5_p0_5 = (q1[5] - p0[5]);
    double q1_6_p0_6 = (q1[6] - p0[6]);
    double q1_7_p0_7 = (q1[7] - p0[7]);
    double a11;
    a11 = (2 * ((((((((p1_0_p0_0 * q1_0_p0_0) + (p1_1_p0_1 * q1_1_p0_1)) + (p1_2_p0_2 * q1_2_p0_2)) + (p1_3_p0_3 * q1_3_p0_3)) + (p1_4_p0_4 * q1_4_p0_4)) + (p1_5_p0_5 * q1_5_p0_5)) + (p1_6_p0_6 * q1_6_p0_6)) + (p1_7_p0_7 * q1_7_p0_7)));
    double q2_0_p0_0 = (q2[0] - p0[0]);
    double q2_1_p0_1 = (q2[1] - p0[1]);
    double q2_2_p0_2 = (q2[2] - p0[2]);
    double q2_3_p0_3 = (q2[3] - p0[3]);
    double q2_4_p0_4 = (q2[4] - p0[4]);
    double q2_5_p0_5 = (q2[5] - p0[5]);
    double q2_6_p0_6 = (q2[6] - p0[6]);
    double q2_7_p0_7 = (q2[7] - p0[7]);
    double a12;
    a12 = (2 * ((((((((p1_0_p0_0 * q2_0_p0_0) + (p1_1_p0_1 * q2_1_p0_1)) + (p1_2_p0_2 * q2_2_p0_2)) + (p1_3_p0_3 * q2_3_p0_3)) + (p1_4_p0_4 * q2_4_p0_4)) + (p1_5_p0_5 * q2_5_p0_5)) + (p1_6_p0_6 * q2_6_p0_6)) + (p1_7_p0_7 * q2_7_p0_7)));
    double q3_0_p0_0 = (q3[0] - p0[0]);
    double q3_1_p0_1 = (q3[1] - p0[1]);
    double q3_2_p0_2 = (q3[2] - p0[2]);
    double q3_3_p0_3 = (q3[3] - p0[3]);
    double q3_4_p0_4 = (q3[4] - p0[4]);
    double q3_5_p0_5 = (q3[5] - p0[5]);
    double q3_6_p0_6 = (q3[6] - p0[6]);
    double q3_7_p0_7 = (q3[7] - p0[7]);
    double a13;
    a13 = (2 * ((((((((p1_0_p0_0 * q3_0_p0_0) + (p1_1_p0_1 * q3_1_p0_1)) + (p1_2_p0_2 * q3_2_p0_2)) + (p1_3_p0_3 * q3_3_p0_3)) + (p1_4_p0_4 * q3_4_p0_4)) + (p1_5_p0_5 * q3_5_p0_5)) + (p1_6_p0_6 * q3_6_p0_6)) + (p1_7_p0_7 * q3_7_p0_7)));
    double a20;
    a20 = (2 * ((((((((p2_0_p0_0 * q0_0_p0_0) + (p2_1_p0_1 * q0_1_p0_1)) + (p2_2_p0_2 * q0_2_p0_2)) + (p2_3_p0_3 * q0_3_p0_3)) + (p2_4_p0_4 * q0_4_p0_4)) + (p2_5_p0_5 * q0_5_p0_5)) + (p2_6_p0_6 * q0_6_p0_6)) + (p2_7_p0_7 * q0_7_p0_7)));
    double a21;
    a21 = (2 * ((((((((p2_0_p0_0 * q1_0_p0_0) + (p2_1_p0_1 * q1_1_p0_1)) + (p2_2_p0_2 * q1_2_p0_2)) + (p2_3_p0_3 * q1_3_p0_3)) + (p2_4_p0_4 * q1_4_p0_4)) + (p2_5_p0_5 * q1_5_p0_5)) + (p2_6_p0_6 * q1_6_p0_6)) + (p2_7_p0_7 * q1_7_p0_7)));
    double a22;
    a22 = (2 * ((((((((p2_0_p0_0 * q2_0_p0_0) + (p2_1_p0_1 * q2_1_p0_1)) + (p2_2_p0_2 * q2_2_p0_2)) + (p2_3_p0_3 * q2_3_p0_3)) + (p2_4_p0_4 * q2_4_p0_4)) + (p2_5_p0_5 * q2_5_p0_5)) + (p2_6_p0_6 * q2_6_p0_6)) + (p2_7_p0_7 * q2_7_p0_7)));
    double a23;
    a23 = (2 * ((((((((p2_0_p0_0 * q3_0_p0_0) + (p2_1_p0_1 * q3_1_p0_1)) + (p2_2_p0_2 * q3_2_p0_2)) + (p2_3_p0_3 * q3_3_p0_3)) + (p2_4_p0_4 * q3_4_p0_4)) + (p2_5_p0_5 * q3_5_p0_5)) + (p2_6_p0_6 * q3_6_p0_6)) + (p2_7_p0_7 * q3_7_p0_7)));
    double a30;
    a30 = (2 * ((((((((p3_0_p0_0 * q0_0_p0_0) + (p3_1_p0_1 * q0_1_p0_1)) + (p3_2_p0_2 * q0_2_p0_2)) + (p3_3_p0_3 * q0_3_p0_3)) + (p3_4_p0_4 * q0_4_p0_4)) + (p3_5_p0_5 * q0_5_p0_5)) + (p3_6_p0_6 * q0_6_p0_6)) + (p3_7_p0_7 * q0_7_p0_7)));
    double a31;
    a31 = (2 * ((((((((p3_0_p0_0 * q1_0_p0_0) + (p3_1_p0_1 * q1_1_p0_1)) + (p3_2_p0_2 * q1_2_p0_2)) + (p3_3_p0_3 * q1_3_p0_3)) + (p3_4_p0_4 * q1_4_p0_4)) + (p3_5_p0_5 * q1_5_p0_5)) + (p3_6_p0_6 * q1_6_p0_6)) + (p3_7_p0_7 * q1_7_p0_7)));
    double a32;
    a32 = (2 * ((((((((p3_0_p0_0 * q2_0_p0_0) + (p3_1_p0_1 * q2_1_p0_1)) + (p3_2_p0_2 * q2_2_p0_2)) + (p3_3_p0_3 * q2_3_p0_3)) + (p3_4_p0_4 * q2_4_p0_4)) + (p3_5_p0_5 * q2_5_p0_5)) + (p3_6_p0_6 * q2_6_p0_6)) + (p3_7_p0_7 * q2_7_p0_7)));
    double a33;
    a33 = (2 * ((((((((p3_0_p0_0 * q3_0_p0_0) + (p3_1_p0_1 * q3_1_p0_1)) + (p3_2_p0_2 * q3_2_p0_2)) + (p3_3_p0_3 * q3_3_p0_3)) + (p3_4_p0_4 * q3_4_p0_4)) + (p3_5_p0_5 * q3_5_p0_5)) + (p3_6_p0_6 * q3_6_p0_6)) + (p3_7_p0_7 * q3_7_p0_7)));
    double a40;
    a40 = (2 * ((((((((p4_0_p0_0 * q0_0_p0_0) + (p4_1_p0_1 * q0_1_p0_1)) + (p4_2_p0_2 * q0_2_p0_2)) + (p4_3_p0_3 * q0_3_p0_3)) + (p4_4_p0_4 * q0_4_p0_4)) + (p4_5_p0_5 * q0_5_p0_5)) + (p4_6_p0_6 * q0_6_p0_6)) + (p4_7_p0_7 * q0_7_p0_7)));
    double a41;
    a41 = (2 * ((((((((p4_0_p0_0 * q1_0_p0_0) + (p4_1_p0_1 * q1_1_p0_1)) + (p4_2_p0_2 * q1_2_p0_2)) + (p4_3_p0_3 * q1_3_p0_3)) + (p4_4_p0_4 * q1_4_p0_4)) + (p4_5_p0_5 * q1_5_p0_5)) + (p4_6_p0_6 * q1_6_p0_6)) + (p4_7_p0_7 * q1_7_p0_7)));
    double a42;
    a42 = (2 * ((((((((p4_0_p0_0 * q2_0_p0_0) + (p4_1_p0_1 * q2_1_p0_1)) + (p4_2_p0_2 * q2_2_p0_2)) + (p4_3_p0_3 * q2_3_p0_3)) + (p4_4_p0_4 * q2_4_p0_4)) + (p4_5_p0_5 * q2_5_p0_5)) + (p4_6_p0_6 * q2_6_p0_6)) + (p4_7_p0_7 * q2_7_p0_7)));
    double a43;
    a43 = (2 * ((((((((p4_0_p0_0 * q3_0_p0_0) + (p4_1_p0_1 * q3_1_p0_1)) + (p4_2_p0_2 * q3_2_p0_2)) + (p4_3_p0_3 * q3_3_p0_3)) + (p4_4_p0_4 * q3_4_p0_4)) + (p4_5_p0_5 * q3_5_p0_5)) + (p4_6_p0_6 * q3_6_p0_6)) + (p4_7_p0_7 * q3_7_p0_7)));
    double b00;
    b00 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double b01;
    b01 = -((((a22 * a33) - (a23 * a32)) + ((a23 * a31) - (a21 * a33))) + ((a21 * a32) - (a22 * a31)));
    double b02;
    b02 = ((((a12 * a33) - (a13 * a32)) + ((a13 * a31) - (a11 * a33))) + ((a11 * a32) - (a12 * a31)));
    double b03;
    b03 = -((((a12 * a23) - (a13 * a22)) + ((a13 * a21) - (a11 * a23))) + ((a11 * a22) - (a12 * a21)));
    double b10;
    b10 = -(((a10 * ((a22 * a33) - (a23 * a32))) - (a20 * ((a12 * a33) - (a13 * a32)))) + (a30 * ((a12 * a23) - (a13 * a22))));
    double b11;
    b11 = ((((a22 * a33) - (a23 * a32)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a32) - (a22 * a30)));
    double b12;
    b12 = -((((a12 * a33) - (a13 * a32)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a32) - (a12 * a30)));
    double b13;
    b13 = ((((a12 * a23) - (a13 * a22)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a22) - (a12 * a20)));
    double b20;
    b20 = (((a10 * ((a21 * a33) - (a23 * a31))) - (a20 * ((a11 * a33) - (a13 * a31)))) + (a30 * ((a11 * a23) - (a13 * a21))));
    double b21;
    b21 = -((((a21 * a33) - (a23 * a31)) + ((a23 * a30) - (a20 * a33))) + ((a20 * a31) - (a21 * a30)));
    double b22;
    b22 = ((((a11 * a33) - (a13 * a31)) + ((a13 * a30) - (a10 * a33))) + ((a10 * a31) - (a11 * a30)));
    double b23;
    b23 = -((((a11 * a23) - (a13 * a21)) + ((a13 * a20) - (a10 * a23))) + ((a10 * a21) - (a11 * a20)));
    double b30;
    b30 = -(((a10 * ((a21 * a32) - (a22 * a31))) - (a20 * ((a11 * a32) - (a12 * a31)))) + (a30 * ((a11 * a22) - (a12 * a21))));
    double b31;
    b31 = ((((a21 * a32) - (a22 * a31)) + ((a22 * a30) - (a20 * a32))) + ((a20 * a31) - (a21 * a30)));
    double b32;
    b32 = -((((a11 * a32) - (a12 * a31)) + ((a12 * a30) - (a10 * a32))) + ((a10 * a31) - (a11 * a30)));
    double b33;
    b33 = ((((a11 * a22) - (a12 * a21)) + ((a12 * a20) - (a10 * a22))) + ((a10 * a21) - (a11 * a20)));
    double Delta;
    Delta = (((b00 + b10) + b20) + b30);
    double DeltaLambda0;
    DeltaLambda0 = ((((b01 * l1) + (b02 * l2)) + (b03 * l3)) + b00);
    double DeltaLambda1;
    DeltaLambda1 = ((((b11 * l1) + (b12 * l2)) + (b13 * l3)) + b10);
    double DeltaLambda2;
    DeltaLambda2 = ((((b21 * l1) + (b22 * l2)) + (b23 * l3)) + b20);
    double DeltaLambda3;
    DeltaLambda3 = ((((b31 * l1) + (b32 * l2)) + (b33 * l3)) + b30);
    double r;
    r = ((Delta * l4) - ((((a40 * DeltaLambda0) + (a41 * DeltaLambda1)) + (a42 * DeltaLambda2)) + (a43 * DeltaLambda3)));
    double eps;
    double max1 = fabs(p2_5_p0_5);
    if( (max1 < fabs(p2_3_p0_3)) )
    {
        max1 = fabs(p2_3_p0_3);
    } 
    if( (max1 < fabs(p2_0_p0_0)) )
    {
        max1 = fabs(p2_0_p0_0);
    } 
    if( (max1 < fabs(p2_1_p0_1)) )
    {
        max1 = fabs(p2_1_p0_1);
    } 
    if( (max1 < fabs(p2_6_p0_6)) )
    {
        max1 = fabs(p2_6_p0_6);
    } 
    if( (max1 < fabs(p2_2_p0_2)) )
    {
        max1 = fabs(p2_2_p0_2);
    } 
    if( (max1 < fabs(p2_4_p0_4)) )
    {
        max1 = fabs(p2_4_p0_4);
    } 
    if( (max1 < fabs(p2_7_p0_7)) )
    {
        max1 = fabs(p2_7_p0_7);
    } 
    double max2 = fabs(p1_4_p0_4);
    if( (max2 < fabs(p1_3_p0_3)) )
    {
        max2 = fabs(p1_3_p0_3);
    } 
    if( (max2 < fabs(p1_7_p0_7)) )
    {
        max2 = fabs(p1_7_p0_7);
    } 
    if( (max2 < fabs(p1_0_p0_0)) )
    {
        max2 = fabs(p1_0_p0_0);
    } 
    if( (max2 < fabs(p1_2_p0_2)) )
    {
        max2 = fabs(p1_2_p0_2);
    } 
    if( (max2 < fabs(p1_5_p0_5)) )
    {
        max2 = fabs(p1_5_p0_5);
    } 
    if( (max2 < fabs(p1_1_p0_1)) )
    {
        max2 = fabs(p1_1_p0_1);
    } 
    if( (max2 < fabs(p1_6_p0_6)) )
    {
        max2 = fabs(p1_6_p0_6);
    } 
    double max3 = fabs(p3_3_p0_3);
    if( (max3 < fabs(p3_0_p0_0)) )
    {
        max3 = fabs(p3_0_p0_0);
    } 
    if( (max3 < fabs(p3_1_p0_1)) )
    {
        max3 = fabs(p3_1_p0_1);
    } 
    if( (max3 < fabs(p3_2_p0_2)) )
    {
        max3 = fabs(p3_2_p0_2);
    } 
    if( (max3 < fabs(p3_4_p0_4)) )
    {
        max3 = fabs(p3_4_p0_4);
    } 
    if( (max3 < fabs(p3_5_p0_5)) )
    {
        max3 = fabs(p3_5_p0_5);
    } 
    if( (max3 < fabs(p3_6_p0_6)) )
    {
        max3 = fabs(p3_6_p0_6);
    } 
    if( (max3 < fabs(p3_7_p0_7)) )
    {
        max3 = fabs(p3_7_p0_7);
    } 
    double max4 = fabs(q0_0_p0_0);
    if( (max4 < fabs(q0_1_p0_1)) )
    {
        max4 = fabs(q0_1_p0_1);
    } 
    if( (max4 < fabs(q0_2_p0_2)) )
    {
        max4 = fabs(q0_2_p0_2);
    } 
    if( (max4 < fabs(q0_3_p0_3)) )
    {
        max4 = fabs(q0_3_p0_3);
    } 
    if( (max4 < fabs(q0_4_p0_4)) )
    {
        max4 = fabs(q0_4_p0_4);
    } 
    if( (max4 < fabs(q0_5_p0_5)) )
    {
        max4 = fabs(q0_5_p0_5);
    } 
    if( (max4 < fabs(q0_6_p0_6)) )
    {
        max4 = fabs(q0_6_p0_6);
    } 
    if( (max4 < fabs(q0_7_p0_7)) )
    {
        max4 = fabs(q0_7_p0_7);
    } 
    if( (max4 < fabs(q1_0_p0_0)) )
    {
        max4 = fabs(q1_0_p0_0);
    } 
    if( (max4 < fabs(q1_1_p0_1)) )
    {
        max4 = fabs(q1_1_p0_1);
    } 
    if( (max4 < fabs(q1_2_p0_2)) )
    {
        max4 = fabs(q1_2_p0_2);
    } 
    if( (max4 < fabs(q1_3_p0_3)) )
    {
        max4 = fabs(q1_3_p0_3);
    } 
    if( (max4 < fabs(q1_4_p0_4)) )
    {
        max4 = fabs(q1_4_p0_4);
    } 
    if( (max4 < fabs(q1_5_p0_5)) )
    {
        max4 = fabs(q1_5_p0_5);
    } 
    if( (max4 < fabs(q1_6_p0_6)) )
    {
        max4 = fabs(q1_6_p0_6);
    } 
    if( (max4 < fabs(q1_7_p0_7)) )
    {
        max4 = fabs(q1_7_p0_7);
    } 
    double max5 = fabs(q1_0_p0_0);
    if( (max5 < fabs(q1_1_p0_1)) )
    {
        max5 = fabs(q1_1_p0_1);
    } 
    if( (max5 < fabs(q1_2_p0_2)) )
    {
        max5 = fabs(q1_2_p0_2);
    } 
    if( (max5 < fabs(q1_3_p0_3)) )
    {
        max5 = fabs(q1_3_p0_3);
    } 
    if( (max5 < fabs(q1_4_p0_4)) )
    {
        max5 = fabs(q1_4_p0_4);
    } 
    if( (max5 < fabs(q1_5_p0_5)) )
    {
        max5 = fabs(q1_5_p0_5);
    } 
    if( (max5 < fabs(q1_6_p0_6)) )
    {
        max5 = fabs(q1_6_p0_6);
    } 
    if( (max5 < fabs(q1_7_p0_7)) )
    {
        max5 = fabs(q1_7_p0_7);
    } 
    if( (max5 < fabs(q2_0_p0_0)) )
    {
        max5 = fabs(q2_0_p0_0);
    } 
    if( (max5 < fabs(q2_1_p0_1)) )
    {
        max5 = fabs(q2_1_p0_1);
    } 
    if( (max5 < fabs(q2_2_p0_2)) )
    {
        max5 = fabs(q2_2_p0_2);
    } 
    if( (max5 < fabs(q2_3_p0_3)) )
    {
        max5 = fabs(q2_3_p0_3);
    } 
    if( (max5 < fabs(q2_4_p0_4)) )
    {
        max5 = fabs(q2_4_p0_4);
    } 
    if( (max5 < fabs(q2_5_p0_5)) )
    {
        max5 = fabs(q2_5_p0_5);
    } 
    if( (max5 < fabs(q2_6_p0_6)) )
    {
        max5 = fabs(q2_6_p0_6);
    } 
    if( (max5 < fabs(q2_7_p0_7)) )
    {
        max5 = fabs(q2_7_p0_7);
    } 
    double max6 = fabs(q2_0_p0_0);
    if( (max6 < fabs(q2_1_p0_1)) )
    {
        max6 = fabs(q2_1_p0_1);
    } 
    if( (max6 < fabs(q2_2_p0_2)) )
    {
        max6 = fabs(q2_2_p0_2);
    } 
    if( (max6 < fabs(q2_3_p0_3)) )
    {
        max6 = fabs(q2_3_p0_3);
    } 
    if( (max6 < fabs(q2_4_p0_4)) )
    {
        max6 = fabs(q2_4_p0_4);
    } 
    if( (max6 < fabs(q2_5_p0_5)) )
    {
        max6 = fabs(q2_5_p0_5);
    } 
    if( (max6 < fabs(q2_6_p0_6)) )
    {
        max6 = fabs(q2_6_p0_6);
    } 
    if( (max6 < fabs(q2_7_p0_7)) )
    {
        max6 = fabs(q2_7_p0_7);
    } 
    if( (max6 < fabs(q3_0_p0_0)) )
    {
        max6 = fabs(q3_0_p0_0);
    } 
    if( (max6 < fabs(q3_1_p0_1)) )
    {
        max6 = fabs(q3_1_p0_1);
    } 
    if( (max6 < fabs(q3_2_p0_2)) )
    {
        max6 = fabs(q3_2_p0_2);
    } 
    if( (max6 < fabs(q3_3_p0_3)) )
    {
        max6 = fabs(q3_3_p0_3);
    } 
    if( (max6 < fabs(q3_4_p0_4)) )
    {
        max6 = fabs(q3_4_p0_4);
    } 
    if( (max6 < fabs(q3_5_p0_5)) )
    {
        max6 = fabs(q3_5_p0_5);
    } 
    if( (max6 < fabs(q3_6_p0_6)) )
    {
        max6 = fabs(q3_6_p0_6);
    } 
    if( (max6 < fabs(q3_7_p0_7)) )
    {
        max6 = fabs(q3_7_p0_7);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta_sign;
    int int_tmp_result;
    lower_bound_1 = max4;
    upper_bound_1 = max4;
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    else 
    {
        if( (max1 > upper_bound_1) )
        {
            upper_bound_1 = max1;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (lower_bound_1 < 2.82528483194754087282e-50) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (4.37492894694731040560e-11 * (((((max2 * max4) * max1) * max5) * max3) * max6));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max7 = max1;
    if( (max7 < max3) )
    {
        max7 = max3;
    } 
    double max8 = max2;
    if( (max8 < fabs(p4_3_p0_3)) )
    {
        max8 = fabs(p4_3_p0_3);
    } 
    if( (max8 < fabs(p4_1_p0_1)) )
    {
        max8 = fabs(p4_1_p0_1);
    } 
    if( (max8 < fabs(p4_4_p0_4)) )
    {
        max8 = fabs(p4_4_p0_4);
    } 
    if( (max8 < fabs(p4_0_p0_0)) )
    {
        max8 = fabs(p4_0_p0_0);
    } 
    if( (max8 < fabs(p4_2_p0_2)) )
    {
        max8 = fabs(p4_2_p0_2);
    } 
    if( (max8 < fabs(p4_5_p0_5)) )
    {
        max8 = fabs(p4_5_p0_5);
    } 
    if( (max8 < fabs(p4_6_p0_6)) )
    {
        max8 = fabs(p4_6_p0_6);
    } 
    if( (max8 < fabs(p4_7_p0_7)) )
    {
        max8 = fabs(p4_7_p0_7);
    } 
    if( (max7 < max8) )
    {
        max7 = max8;
    } 
    if( (max7 < max2) )
    {
        max7 = max2;
    } 
    if( (max7 < max6) )
    {
        max7 = max6;
    } 
    double max9 = max8;
    if( (max9 < max2) )
    {
        max9 = max2;
    } 
    if( (max9 < max5) )
    {
        max9 = max5;
    } 
    double max10 = max4;
    double max11 = max4;
    if( (max11 < max5) )
    {
        max11 = max5;
    } 
    if( (max10 < max11) )
    {
        max10 = max11;
    } 
    if( (max10 < max2) )
    {
        max10 = max2;
    } 
    if( (max10 < max6) )
    {
        max10 = max6;
    } 
    if( (max10 < max5) )
    {
        max10 = max5;
    } 
    lower_bound_1 = max11;
    upper_bound_1 = max11;
    if( (max9 < lower_bound_1) )
    {
        lower_bound_1 = max9;
    } 
    else 
    {
        if( (max9 > upper_bound_1) )
        {
            upper_bound_1 = max9;
        } 
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (max1 < lower_bound_1) )
    {
        lower_bound_1 = max1;
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    if( (max8 < lower_bound_1) )
    {
        lower_bound_1 = max8;
    } 
    if( (max10 < lower_bound_1) )
    {
        lower_bound_1 = max10;
    } 
    else 
    {
        if( (max10 > upper_bound_1) )
        {
            upper_bound_1 = max10;
        } 
    } 
    if( (lower_bound_1 < 4.17402518597284772324e-38) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 4.83570327845851562508e+24) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (2.41492645607254025015e-09 * (((((((max8 * max11) * max1) * max10) * max3) * max10) * max9) * max7));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta_sign * int_tmp_result_FFWKCAA);
} 


/******* extracted from ../numerics/predicates/side4h.h *******/

inline int side4h_3d_filter( const double* p0, const double* p1, const double* p2, const double* p3, const double* p4, double h0, double h1, double h2, double h3, double h4) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (p1[2] - p0[2]);
    double a14;
    a14 = (h0 - h1);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (p2[2] - p0[2]);
    double a24;
    a24 = (h0 - h2);
    double a31;
    a31 = (p3[0] - p0[0]);
    double a32;
    a32 = (p3[1] - p0[1]);
    double a33;
    a33 = (p3[2] - p0[2]);
    double a34;
    a34 = (h0 - h3);
    double a41;
    a41 = (p4[0] - p0[0]);
    double a42;
    a42 = (p4[1] - p0[1]);
    double a43;
    a43 = (p4[2] - p0[2]);
    double a44;
    a44 = (h0 - h4);
    double Delta1;
    Delta1 = (((a21 * ((a32 * a43) - (a33 * a42))) - (a31 * ((a22 * a43) - (a23 * a42)))) + (a41 * ((a22 * a33) - (a23 * a32))));
    double Delta2;
    Delta2 = (((a11 * ((a32 * a43) - (a33 * a42))) - (a31 * ((a12 * a43) - (a13 * a42)))) + (a41 * ((a12 * a33) - (a13 * a32))));
    double Delta3;
    Delta3 = (((a11 * ((a22 * a43) - (a23 * a42))) - (a21 * ((a12 * a43) - (a13 * a42)))) + (a41 * ((a12 * a23) - (a13 * a22))));
    double Delta4;
    Delta4 = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    double r;
    r = ((((Delta1 * a14) - (Delta2 * a24)) + (Delta3 * a34)) - (Delta4 * a44));
    double eps;
    double max1 = fabs(a11);
    if( (max1 < fabs(a21)) )
    {
        max1 = fabs(a21);
    } 
    if( (max1 < fabs(a31)) )
    {
        max1 = fabs(a31);
    } 
    double max2 = fabs(a12);
    if( (max2 < fabs(a13)) )
    {
        max2 = fabs(a13);
    } 
    if( (max2 < fabs(a22)) )
    {
        max2 = fabs(a22);
    } 
    if( (max2 < fabs(a23)) )
    {
        max2 = fabs(a23);
    } 
    double max3 = fabs(a22);
    if( (max3 < fabs(a23)) )
    {
        max3 = fabs(a23);
    } 
    if( (max3 < fabs(a32)) )
    {
        max3 = fabs(a32);
    } 
    if( (max3 < fabs(a33)) )
    {
        max3 = fabs(a33);
    } 
    double lower_bound_1;
    double upper_bound_1;
    int Delta4_sign;
    int int_tmp_result;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.63288018496748314939e-98) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 7.23700557733225980357e+75) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (5.11071278299732992696e-15 * ((max2 * max3) * max1));
        if( (Delta4 > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta4 < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    Delta4_sign = int_tmp_result;
    int int_tmp_result_FFWKCAA;
    double max4 = max1;
    if( (max4 < fabs(a41)) )
    {
        max4 = fabs(a41);
    } 
    double max5 = max2;
    if( (max5 < max3) )
    {
        max5 = max3;
    } 
    double max6 = fabs(a14);
    if( (max6 < fabs(a24)) )
    {
        max6 = fabs(a24);
    } 
    if( (max6 < fabs(a34)) )
    {
        max6 = fabs(a34);
    } 
    if( (max6 < fabs(a44)) )
    {
        max6 = fabs(a44);
    } 
    double max7 = max3;
    if( (max7 < fabs(a42)) )
    {
        max7 = fabs(a42);
    } 
    if( (max7 < fabs(a43)) )
    {
        max7 = fabs(a43);
    } 
    lower_bound_1 = max4;
    upper_bound_1 = max4;
    if( (max5 < lower_bound_1) )
    {
        lower_bound_1 = max5;
    } 
    else 
    {
        if( (max5 > upper_bound_1) )
        {
            upper_bound_1 = max5;
        } 
    } 
    if( (max6 < lower_bound_1) )
    {
        lower_bound_1 = max6;
    } 
    else 
    {
        if( (max6 > upper_bound_1) )
        {
            upper_bound_1 = max6;
        } 
    } 
    if( (max7 < lower_bound_1) )
    {
        lower_bound_1 = max7;
    } 
    else 
    {
        if( (max7 > upper_bound_1) )
        {
            upper_bound_1 = max7;
        } 
    } 
    if( (lower_bound_1 < 2.89273249588395194294e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 7.23700557733225980357e+75) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (3.17768858673611390687e-14 * (((max5 * max7) * max4) * max6));
        if( (r > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (r < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return (Delta4_sign * int_tmp_result_FFWKCAA);
} 


/******* extracted from ../numerics/predicates/orient2d.h *******/

inline int orient_2d_filter( const double* p0, const double* p1, const double* p2) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double Delta;
    Delta = ((a11 * a22) - (a12 * a21));
    int int_tmp_result;
    double eps;
    double max1 = fabs(a11);
    if( (max1 < fabs(a12)) )
    {
        max1 = fabs(a12);
    } 
    double max2 = fabs(a21);
    if( (max2 < fabs(a22)) )
    {
        max2 = fabs(a22);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 5.00368081960964635413e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.67597599124282407923e+153) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.88720573725927976811e-16 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


/******* extracted from ../numerics/predicates/orient3d.h *******/

inline int orient_3d_filter(const double* p0, const double* p1, const double* p2, const double* p3) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (p1[2] - p0[2]);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (p2[2] - p0[2]);
    double a31;
    a31 = (p3[0] - p0[0]);
    double a32;
    a32 = (p3[1] - p0[1]);
    double a33;
    a33 = (p3[2] - p0[2]);
    double Delta;
    Delta = (((a11 * ((a22 * a33) - (a23 * a32))) - (a21 * ((a12 * a33) - (a13 * a32)))) + (a31 * ((a12 * a23) - (a13 * a22))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(a11);
    if((max1 < fabs(a21)))
    {
        max1 = fabs(a21);
    }
    if((max1 < fabs(a31)))
    {
        max1 = fabs(a31);
    }
    double max2 = fabs(a12);
    if((max2 < fabs(a13)))
    {
        max2 = fabs(a13);
    }
    if((max2 < fabs(a22)))
    {
        max2 = fabs(a22);
    }
    if((max2 < fabs(a23)))
    {
        max2 = fabs(a23);
    }
    double max3 = fabs(a22);
    if((max3 < fabs(a23)))
    {
        max3 = fabs(a23);
    }
    if((max3 < fabs(a32)))
    {
        max3 = fabs(a32);
    }
    if((max3 < fabs(a33)))
    {
        max3 = fabs(a33);
    }
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if((max2 < lower_bound_1))
    {
        lower_bound_1 = max2;
    }
    else
    {
        if((max2 > upper_bound_1))
        {
            upper_bound_1 = max2;
        }
    }
    if((max3 < lower_bound_1))
    {
        lower_bound_1 = max3;
    }
    else
    {
        if((max3 > upper_bound_1))
        {
            upper_bound_1 = max3;
        }
    }
    if((lower_bound_1 < 1.63288018496748314939e-98))
    {
        return FPG_UNCERTAIN_VALUE;
    }
    else
    {
        if((upper_bound_1 > 5.59936185544450928309e+101))
        {
            return FPG_UNCERTAIN_VALUE;
        }
        eps = (5.11071278299732992696e-15 * ((max2 * max3) * max1));
        if((Delta > eps))
        {
            int_tmp_result = 1;
        }
        else
        {
            if((Delta < -eps))
            {
                int_tmp_result = -1;
            }
            else
            {
                return FPG_UNCERTAIN_VALUE;
            }
        }
    }
    return int_tmp_result;
}


/******* extracted from ../numerics/predicates/dot3d.h *******/

inline int dot_3d_filter( const double* p0, const double* p1, const double* p2) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (p1[2] - p0[2]);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (p2[2] - p0[2]);
    double Delta;
    Delta = (((a11 * a21) + (a12 * a22)) + (a13 * a23));
    int int_tmp_result;
    double eps;
    double max1 = fabs(a11);
    if( (max1 < fabs(a12)) )
    {
        max1 = fabs(a12);
    } 
    if( (max1 < fabs(a13)) )
    {
        max1 = fabs(a13);
    } 
    double max2 = fabs(a21);
    if( (max2 < fabs(a22)) )
    {
        max2 = fabs(a22);
    } 
    if( (max2 < fabs(a23)) )
    {
        max2 = fabs(a23);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 3.78232824369468524638e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.67597599124282407923e+153) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (1.55534235888797977480e-15 * (max1 * max2));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 

/******* extracted from ../numerics/predicates/det3d.h *******/

inline int det_3d_filter( const double* p0, const double* p1, const double* p2) {
    double Delta;
    Delta = (((p0[0] * ((p1[1] * p2[2]) - (p1[2] * p2[1]))) - (p1[0] * ((p0[1] * p2[2]) - (p0[2] * p2[1])))) + (p2[0] * ((p0[1] * p1[2]) - (p0[2] * p1[1]))));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0[0]);
    if( (max1 < fabs(p1[0])) )
    {
        max1 = fabs(p1[0]);
    } 
    if( (max1 < fabs(p2[0])) )
    {
        max1 = fabs(p2[0]);
    } 
    double max2 = fabs(p0[1]);
    if( (max2 < fabs(p0[2])) )
    {
        max2 = fabs(p0[2]);
    } 
    if( (max2 < fabs(p1[1])) )
    {
        max2 = fabs(p1[1]);
    } 
    if( (max2 < fabs(p1[2])) )
    {
        max2 = fabs(p1[2]);
    } 
    double max3 = fabs(p1[1]);
    if( (max3 < fabs(p1[2])) )
    {
        max3 = fabs(p1[2]);
    } 
    if( (max3 < fabs(p2[1])) )
    {
        max3 = fabs(p2[1]);
    } 
    if( (max3 < fabs(p2[2])) )
    {
        max3 = fabs(p2[2]);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 1.92663387981871579179e-98) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.11987237108890185662e+102) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (3.11133555671680765034e-15 * ((max2 * max3) * max1));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 

/******* extracted from ../numerics/predicates/det4d.h *******/

inline int det_4d_filter( const double* p0, const double* p1, const double* p2, const double* p3) {
    double m12;
    m12 = ((p1[0] * p0[1]) - (p0[0] * p1[1]));
    double m13;
    m13 = ((p2[0] * p0[1]) - (p0[0] * p2[1]));
    double m14;
    m14 = ((p3[0] * p0[1]) - (p0[0] * p3[1]));
    double m23;
    m23 = ((p2[0] * p1[1]) - (p1[0] * p2[1]));
    double m24;
    m24 = ((p3[0] * p1[1]) - (p1[0] * p3[1]));
    double m34;
    m34 = ((p3[0] * p2[1]) - (p2[0] * p3[1]));
    double m123;
    m123 = (((m23 * p0[2]) - (m13 * p1[2])) + (m12 * p2[2]));
    double m124;
    m124 = (((m24 * p0[2]) - (m14 * p1[2])) + (m12 * p3[2]));
    double m134;
    m134 = (((m34 * p0[2]) - (m14 * p2[2])) + (m13 * p3[2]));
    double m234;
    m234 = (((m34 * p1[2]) - (m24 * p2[2])) + (m23 * p3[2]));
    double Delta;
    Delta = ((((m234 * p0[3]) - (m134 * p1[3])) + (m124 * p2[3])) - (m123 * p3[3]));
    int int_tmp_result;
    double eps;
    double max1 = fabs(p0[0]);
    if( (max1 < fabs(p1[0])) )
    {
        max1 = fabs(p1[0]);
    } 
    if( (max1 < fabs(p2[0])) )
    {
        max1 = fabs(p2[0]);
    } 
    if( (max1 < fabs(p3[0])) )
    {
        max1 = fabs(p3[0]);
    } 
    double max2 = fabs(p0[1]);
    if( (max2 < fabs(p1[1])) )
    {
        max2 = fabs(p1[1]);
    } 
    if( (max2 < fabs(p2[1])) )
    {
        max2 = fabs(p2[1]);
    } 
    if( (max2 < fabs(p3[1])) )
    {
        max2 = fabs(p3[1]);
    } 
    double max3 = fabs(p0[2]);
    if( (max3 < fabs(p1[2])) )
    {
        max3 = fabs(p1[2]);
    } 
    if( (max3 < fabs(p2[2])) )
    {
        max3 = fabs(p2[2]);
    } 
    if( (max3 < fabs(p3[2])) )
    {
        max3 = fabs(p3[2]);
    } 
    double max4 = fabs(p0[3]);
    if( (max4 < fabs(p1[3])) )
    {
        max4 = fabs(p1[3]);
    } 
    if( (max4 < fabs(p2[3])) )
    {
        max4 = fabs(p2[3]);
    } 
    if( (max4 < fabs(p3[3])) )
    {
        max4 = fabs(p3[3]);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (max4 < lower_bound_1) )
    {
        lower_bound_1 = max4;
    } 
    else 
    {
        if( (max4 > upper_bound_1) )
        {
            upper_bound_1 = max4;
        } 
    } 
    if( (lower_bound_1 < 3.20402459074399025456e-74) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.44740111546645196071e+76) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (2.11135406605316806158e-14 * (((max1 * max2) * max3) * max4));
        if( (Delta > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (Delta < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return int_tmp_result;
} 


/******* extracted from ../numerics/predicates/aligned3d.h *******/

inline int aligned_3d_filter( const double* p0, const double* p1, const double* p2) {
    double a11;
    a11 = (p1[0] - p0[0]);
    double a12;
    a12 = (p1[1] - p0[1]);
    double a13;
    a13 = (p1[2] - p0[2]);
    double a21;
    a21 = (p2[0] - p0[0]);
    double a22;
    a22 = (p2[1] - p0[1]);
    double a23;
    a23 = (p2[2] - p0[2]);
    double delta1;
    delta1 = ((a12 * a23) - (a22 * a13));
    double delta2;
    delta2 = ((a13 * a21) - (a23 * a11));
    double delta3;
    delta3 = ((a11 * a22) - (a21 * a12));
    int int_tmp_result;
    double eps;
    int int_tmp_result_FFWKCAA;
    int int_tmp_result_k60Ocge;
    double max1 = fabs(a12);
    if( (max1 < fabs(a22)) )
    {
        max1 = fabs(a22);
    } 
    double max2 = fabs(a13);
    if( (max2 < fabs(a23)) )
    {
        max2 = fabs(a23);
    } 
    double lower_bound_1;
    double upper_bound_1;
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 5.00368081960964635413e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.67597599124282407923e+153) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.88720573725927976811e-16 * (max1 * max2));
        if( (delta1 > eps) )
        {
            int_tmp_result = 1;
        } 
        else 
        {
            if( (delta1 < -eps) )
            {
                int_tmp_result = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    double max3 = fabs(a11);
    if( (max3 < fabs(a21)) )
    {
        max3 = fabs(a21);
    } 
    lower_bound_1 = max3;
    upper_bound_1 = max3;
    if( (max2 < lower_bound_1) )
    {
        lower_bound_1 = max2;
    } 
    else 
    {
        if( (max2 > upper_bound_1) )
        {
            upper_bound_1 = max2;
        } 
    } 
    if( (lower_bound_1 < 5.00368081960964635413e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.67597599124282407923e+153) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.88720573725927976811e-16 * (max2 * max3));
        if( (delta2 > eps) )
        {
            int_tmp_result_FFWKCAA = 1;
        } 
        else 
        {
            if( (delta2 < -eps) )
            {
                int_tmp_result_FFWKCAA = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    lower_bound_1 = max1;
    upper_bound_1 = max1;
    if( (max3 < lower_bound_1) )
    {
        lower_bound_1 = max3;
    } 
    else 
    {
        if( (max3 > upper_bound_1) )
        {
            upper_bound_1 = max3;
        } 
    } 
    if( (lower_bound_1 < 5.00368081960964635413e-147) )
    {
        return FPG_UNCERTAIN_VALUE;
    } 
    else 
    {
        if( (upper_bound_1 > 1.67597599124282407923e+153) )
        {
            return FPG_UNCERTAIN_VALUE;
        } 
        eps = (8.88720573725927976811e-16 * (max3 * max1));
        if( (delta3 > eps) )
        {
            int_tmp_result_k60Ocge = 1;
        } 
        else 
        {
            if( (delta3 < -eps) )
            {
                int_tmp_result_k60Ocge = -1;
            } 
            else 
            {
                return FPG_UNCERTAIN_VALUE;
            } 
        } 
    } 
    return ((((int_tmp_result == 0) && (int_tmp_result_FFWKCAA == 0)) && (int_tmp_result_k60Ocge == 0)) ? 0 : 1);
} 

/******* extracted from ../numerics/predicates.cpp *******/


// This makes sure the compiler will not optimize y = a*x+b
// with fused multiply-add, this would break the exact
// predicates.
#ifdef GEO_COMPILER_MSVC
#pragma fp_contract(off)
#endif

#include <algorithm>

#define FPG_UNCERTAIN_VALUE 0


#ifdef __SSE2__ 
#include <emmintrin.h>
#endif

#ifdef __AVX2__
#include <immintrin.h>
#endif

namespace {

    using namespace GEO;
    
    GEO::PCK::SOSMode SOS_mode_ = GEO::PCK::SOS_ADDRESS; 

    class LexicoCompare {
    public:

	LexicoCompare(index_t dim) : dim_(dim) {
	}

	bool operator()(const double* x, const double* y) const {
	    for(index_t i=0; i<dim_-1; ++i) {
		if(x[i] < y[i]) {
		    return true;
		}
		if(x[i] > y[i]) {
		    return false;
		}
	    }
	    return (x[dim_-1] < y[dim_-1]);
	}
    private:
	index_t dim_;
    };

    bool lexico_compare_3d(const double* x, const double* y) {
	if(x[0] < y[0]) {
	    return true;
	}
	if(x[0] > y[0]) {
	    return false;
	}
	if(x[1] < y[1]) {
	    return true;
	}
	if(x[1] > y[1]) {
	    return false;
	}
	return x[2] < y[2];
    }

    void SOS_sort(const double** begin, const double** end, index_t dim) {
	if(SOS_mode_ == PCK::SOS_ADDRESS) {
	    std::sort(begin, end);
	} else {
	    if(dim == 3) {
		std::sort(begin, end, lexico_compare_3d);
	    } else {
		std::sort(begin, end, LexicoCompare(dim));
	    }
	}
    }

    inline double max4(double x1, double x2, double x3, double x4) {
#ifdef __SSE2__
	double result;
	__m128d X1 =_mm_load_sd(&x1);
	__m128d X2 =_mm_load_sd(&x2);
	__m128d X3 =_mm_load_sd(&x3);
	__m128d X4 =_mm_load_sd(&x4);
	X1 = _mm_max_sd(X1,X2);
	X3 = _mm_max_sd(X3,X4);
	X1 = _mm_max_sd(X1,X3);
	_mm_store_sd(&result, X1);
	return result;
#else	
	return std::max(std::max(x1,x2),std::max(x3,x4));
#endif	
    }


    inline void get_minmax3(
	double& m, double& M, double x1, double x2, double x3
    ) {
#ifdef __SSE2__
	__m128d X1 =_mm_load_sd(&x1);
	__m128d X2 =_mm_load_sd(&x2);
	__m128d X3 =_mm_load_sd(&x3);
	__m128d MIN12 = _mm_min_sd(X1,X2);
	__m128d MAX12 = _mm_max_sd(X1,X2);
	X1 = _mm_min_sd(MIN12, X3);
	X3 = _mm_max_sd(MAX12, X3);
	_mm_store_sd(&m, X1);
	_mm_store_sd(&M, X3);
#else	
	m = std::min(std::min(x1,x2), x3);
	M = std::max(std::max(x1,x2), x3);
#endif	
    }

#ifdef __AVX2__

    inline __m256d avx2_vecdet(__m256d A, __m256d B, __m256d C, __m256d D) {
	__m256d AD = _mm256_mul_pd(A,D);
	__m256d BC = _mm256_mul_pd(B,C);
	return _mm256_sub_pd(AD,BC);
    }

    inline double avx2_det4x4(
	__m256d C11,
	__m256d C12,
	__m256d C13,
	__m256d C14
    ) {
	// We develop w.r.t. the first column and
	// compute the 4 minors simultaneously.
	
	__m256d C41 = _mm256_permute4x64_pd(C11, _MM_SHUFFLE(2,1,0,3)); 
	
	__m256d C22 = _mm256_permute4x64_pd(C12, _MM_SHUFFLE(0,3,2,1)); 
	__m256d C32 = _mm256_permute4x64_pd(C12, _MM_SHUFFLE(1,0,3,2)); 

	__m256d C23 = _mm256_permute4x64_pd(C13, _MM_SHUFFLE(0,3,2,1)); 
	__m256d C33 = _mm256_permute4x64_pd(C13, _MM_SHUFFLE(1,0,3,2)); 

        __m256d C24 = _mm256_permute4x64_pd(C14, _MM_SHUFFLE(0,3,2,1)); 
        __m256d C34 = _mm256_permute4x64_pd(C14, _MM_SHUFFLE(1,0,3,2)); 
	
	__m256d M1 = _mm256_mul_pd(C12,avx2_vecdet(C23,C24,C33,C34));	
	__m256d M2 = _mm256_mul_pd(C22,avx2_vecdet(C13,C14,C33,C34));
	__m256d M3 = _mm256_mul_pd(C32,avx2_vecdet(C13,C14,C23,C24));

	// compute the 4 3x3 minors simulateously by
	// assembling the 3*4 2x2 minors
	__m256d M = _mm256_add_pd(_mm256_sub_pd(M1,M2),M3);

	// multiply the 4 3x3 minors by the 4 coefficients of the
	// first column (permutted so that a41 comes first).
	M = _mm256_mul_pd(M, C41);

	// Compute -m0 +m1 -m2 +m3
	M = _mm256_permute4x64_pd(M, _MM_SHUFFLE(2,0,3,1)); 
	__m128d M_a = _mm256_extractf128_pd(M, 0);
	__m128d M_b = _mm256_extractf128_pd(M, 1);
	__m128d Mab = _mm_sub_pd(M_a,M_b);
	double m[2];
	_mm_store_pd(m, Mab);
	return m[0]+m[1];
    }

    inline int in_sphere_3d_filter_avx2(
        const double* p, const double* q, 
        const double* r, const double* s, const double* t
    ) {

	// Mask to load just three doubles from the points.
	__m256i XYZonly = _mm256_set_epi64x(
	    0,~__int64_t(0),~__int64_t(0),~__int64_t(0)
	);
	__m256d P = _mm256_maskload_pd(p, XYZonly);
	__m256d Q = _mm256_maskload_pd(q, XYZonly);
	__m256d R = _mm256_maskload_pd(r, XYZonly);
	__m256d S = _mm256_maskload_pd(s, XYZonly);
	__m256d T = _mm256_maskload_pd(t, XYZonly);	
	
	__m256d PT = _mm256_sub_pd(P,T);
	__m256d QT = _mm256_sub_pd(Q,T);
	__m256d RT = _mm256_sub_pd(R,T);
	__m256d ST = _mm256_sub_pd(S,T);			

	// Absolute values by masking sign bit.
	__m256d sign_mask = _mm256_set1_pd(-0.);
	__m256d absPT     = _mm256_andnot_pd(PT, sign_mask);
	__m256d absQT     = _mm256_andnot_pd(QT, sign_mask);
	__m256d absRT     = _mm256_andnot_pd(RT, sign_mask);
	__m256d absST     = _mm256_andnot_pd(ST, sign_mask);	
	__m256d maxXYZ    = _mm256_max_pd(
	    _mm256_max_pd(absPT, absQT), _mm256_max_pd(absRT, absST)
	);

	// Separating maxX, maxY, maxZ in three different registers
	__m128d maxX = _mm256_extractf128_pd(maxXYZ,0);
	__m128d maxZ = _mm256_extractf128_pd(maxXYZ,1);
	__m128d maxY = _mm_shuffle_pd(maxX, maxX, 1);
	__m128d max_max = _mm_max_pd(maxX, _mm_max_pd(maxY, maxZ));

	// Computing dynamic filter
	__m128d eps     = _mm_set1_pd(1.2466136531027298e-13);
	        eps     = _mm_mul_pd(eps, _mm_mul_pd(maxX, _mm_mul_pd(maxY, maxZ)));
		eps     = _mm_mul_pd(eps, _mm_mul_pd(max_max, max_max));
	
	// Transpose [PT, QT, RT, ST] -> X,Y,Z (last column is 0, ignored)
	__m256d tmp0 = _mm256_shuffle_pd(PT, QT, 0x0);		
	__m256d tmp2 = _mm256_shuffle_pd(PT, QT, 0xF);		
	__m256d tmp1 = _mm256_shuffle_pd(RT, ST, 0x0);		
	__m256d tmp3 = _mm256_shuffle_pd(RT, ST, 0xF);		
	__m256d X  = _mm256_permute2f128_pd(tmp0, tmp1, 0x20);
	__m256d Y  = _mm256_permute2f128_pd(tmp2, tmp3, 0x20);
	__m256d Z  = _mm256_permute2f128_pd(tmp0, tmp1, 0x31);

	// Compute first column with squared lengths of vectors
	__m256d X2 = _mm256_mul_pd(X,X);
	__m256d Y2 = _mm256_mul_pd(Y,Y);
	__m256d Z2 = _mm256_mul_pd(Z,Z);
	__m256d L2 = _mm256_add_pd(_mm256_add_pd(X2,Y2),Z2);

	double det = avx2_det4x4(L2,X,Y,Z);

	double epsval;
	_mm_store_pd1(&epsval, eps);
	
	// Note: inverted as compared to CGAL
	//   CGAL: in_sphere_3d (called side_of_oriented_sphere())
	//      positive side is outside the sphere.
	//   PCK: in_sphere_3d : positive side is inside the sphere

	return (det > epsval) * -1 + (det < -epsval);
    }
    
#endif
    
    inline int in_sphere_3d_filter_optim(
        const double* p, const double* q, 
        const double* r, const double* s, const double* t
    ) {
        double ptx = p[0] - t[0];
        double pty = p[1] - t[1];
        double ptz = p[2] - t[2];
        double pt2 = geo_sqr(ptx) + geo_sqr(pty) + geo_sqr(ptz);

        double qtx = q[0] - t[0];
        double qty = q[1] - t[1];
        double qtz = q[2] - t[2];
        double qt2 = geo_sqr(qtx) + geo_sqr(qty) + geo_sqr(qtz);

        double rtx = r[0] - t[0];
        double rty = r[1] - t[1];
        double rtz = r[2] - t[2];
        double rt2 = geo_sqr(rtx) + geo_sqr(rty) + geo_sqr(rtz);

        double stx = s[0] - t[0];
        double sty = s[1] - t[1];
        double stz = s[2] - t[2];
        double st2 = geo_sqr(stx) + geo_sqr(sty) + geo_sqr(stz);

        // Compute the semi-static bound.
        double maxx = ::fabs(ptx);
        double maxy = ::fabs(pty);
        double maxz = ::fabs(ptz);
        
        double aqtx = ::fabs(qtx);
        double artx = ::fabs(rtx);
        double astx = ::fabs(stx);
        
        double aqty = ::fabs(qty);
        double arty = ::fabs(rty);
        double asty = ::fabs(sty);
        
        double aqtz = ::fabs(qtz);
        double artz = ::fabs(rtz);
        double astz = ::fabs(stz);

	maxx = max4(maxx, aqtx, artx, astx);
	maxy = max4(maxy, aqty, arty, asty);
	maxz = max4(maxz, aqtz, artz, astz);
	
        double eps = 1.2466136531027298e-13 * maxx * maxy * maxz;

	double min_max;
	double max_max;
	get_minmax3(min_max, max_max, maxx, maxy, maxz);

        double det = det4x4(
                        ptx,pty,ptz,pt2,
                        rtx,rty,rtz,rt2,
                        qtx,qty,qtz,qt2,
                        stx,sty,stz,st2
                     );

        if (min_max < 1e-58)  { /* sqrt^5(min_double/eps) */
            // Protect against underflow in the computation of eps.
            return FPG_UNCERTAIN_VALUE;
        } else if (max_max < 1e61)  { /* sqrt^5(max_double/4 [hadamard]) */
            // Protect against overflow in the computation of det.
            eps *= (max_max * max_max);
            // Note: inverted as compared to CGAL
            //   CGAL: in_sphere_3d (called side_of_oriented_sphere())
            //      positive side is outside the sphere.
            //   PCK: in_sphere_3d : positive side is inside the sphere
            if (det > eps)  return -1;
            if (det < -eps) return  1;
        }

        return FPG_UNCERTAIN_VALUE;
    }


    using namespace GEO;

    index_t cnt_side1_total = 0;
    index_t cnt_side1_exact = 0;
    index_t cnt_side1_SOS = 0;
    index_t len_side1 = 0;

    index_t cnt_side2_total = 0;
    index_t cnt_side2_exact = 0;
    index_t cnt_side2_SOS = 0;
    index_t len_side2_num = 0;
    index_t len_side2_denom = 0;
    index_t len_side2_SOS = 0;

    index_t cnt_side3_total = 0;
    index_t cnt_side3_exact = 0;
    index_t cnt_side3_SOS = 0;
    index_t len_side3_num = 0;
    index_t len_side3_denom = 0;
    index_t len_side3_SOS = 0;

    index_t cnt_side3h_total = 0;
    index_t cnt_side3h_exact = 0;
    index_t cnt_side3h_SOS = 0;
    index_t len_side3h_num = 0;
    index_t len_side3h_denom = 0;
    index_t len_side3h_SOS = 0;
    
    index_t cnt_side4_total = 0;
    index_t cnt_side4_exact = 0;
    index_t cnt_side4_SOS = 0;
    index_t len_side4_num = 0;
    index_t len_side4_denom = 0;
    index_t len_side4_SOS = 0;

    index_t cnt_orient2d_total = 0;
    index_t cnt_orient2d_exact = 0;
    index_t len_orient2d = 0;

    index_t cnt_orient3d_total = 0;
    index_t cnt_orient3d_exact = 0;
    index_t len_orient3d = 0;

    index_t cnt_orient3dh_total = 0;
    index_t cnt_orient3dh_exact = 0;
    index_t cnt_orient3dh_SOS = 0;
    index_t len_orient3dh_num = 0;
    index_t len_orient3dh_denom = 0;
    index_t len_orient3dh_SOS = 0;

    // ================= side1 =========================================

    Sign side1_exact_SOS(
        const double* p0, const double* p1,
        const double* q0,
        coord_index_t dim
    ) {
        cnt_side1_exact++;
        expansion& l = expansion_sq_dist(p0, p1, dim);
        expansion& a = expansion_dot_at(p1, q0, p0, dim).scale_fast(2.0);
        expansion& r = expansion_diff(l, a);
        Sign r_sign = r.sign();
        // Symbolic perturbation, Simulation of Simplicity
        if(r_sign == ZERO) {
            cnt_side1_SOS++;
            return (p0 < p1) ? POSITIVE : NEGATIVE;
        }
        len_side1 = std::max(len_side1, r.length());
        return r_sign;
    }

    Sign side1_3d_SOS(
        const double* p0, const double* p1, const double* q0
    ) {
        Sign result = Sign(side1_3d_filter(p0, p1, q0));
        if(result == ZERO) {
            result = side1_exact_SOS(p0, p1, q0, 3);
        }
        return result;
    }

    Sign side1_4d_SOS(
        const double* p0, const double* p1, const double* q0
    ) {
        Sign result = Sign(side1_4d_filter(p0, p1, q0));
        if(result == ZERO) {
            result = side1_exact_SOS(p0, p1, q0, 4);
        }
        return result;
    }

    Sign side1_6d_SOS(
        const double* p0, const double* p1, const double* q0
    ) {
        Sign result = Sign(side1_6d_filter(p0, p1, q0));
        if(result == ZERO) {
            result = side1_exact_SOS(p0, p1, q0, 6);
        }
        return result;
    }

    Sign side1_7d_SOS(
        const double* p0, const double* p1, const double* q0
    ) {
        Sign result = Sign(side1_7d_filter(p0, p1, q0));
        if(result == ZERO) {
            result = side1_exact_SOS(p0, p1, q0, 7);
        }
        return result;
    }

    Sign side1_8d_SOS(
        const double* p0, const double* p1, const double* q0
    ) {
        Sign result = Sign(side1_8d_filter(p0, p1, q0));
        if(result == ZERO) {
            result = side1_exact_SOS(p0, p1, q0, 8);
        }
        return result;
    }
    
    // ================= side2 =========================================

    Sign side2_exact_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1,
        coord_index_t dim
    ) {
        cnt_side2_exact++;

        const expansion& l1 = expansion_sq_dist(p1, p0, dim);
        const expansion& l2 = expansion_sq_dist(p2, p0, dim);

        const expansion& a10 = expansion_dot_at(p1, q0, p0, dim).scale_fast(2.0);
        const expansion& a11 = expansion_dot_at(p1, q1, p0, dim).scale_fast(2.0);
        const expansion& a20 = expansion_dot_at(p2, q0, p0, dim).scale_fast(2.0);
        const expansion& a21 = expansion_dot_at(p2, q1, p0, dim).scale_fast(2.0);

        const expansion& Delta = expansion_diff(a11, a10);

        Sign Delta_sign = Delta.sign();
        // Should not occur with symbolic
        // perturbation done at previous steps.
        geo_assert(Delta_sign != ZERO);

        //       [ Lambda0 ]   [ -1 ]        [  a11 ]
        // Delta [         ] = [    ] * l1 + [      ]
        //       [ Lambda1 ]   [  1 ]        [ -a10 ]

        const expansion& DeltaLambda0 = expansion_diff(a11, l1);
        const expansion& DeltaLambda1 = expansion_diff(l1, a10);

        // r = Delta*l2 - ( a20*DeltaLambda0 + a21*DeltaLambda1 )

        const expansion& r0 = expansion_product(Delta, l2);
        const expansion& r1 = expansion_product(a20, DeltaLambda0).negate();
        const expansion& r2 = expansion_product(a21, DeltaLambda1).negate();
        const expansion& r = expansion_sum3(r0, r1, r2);

        Sign r_sign = r.sign();

        // Statistics
        len_side2_num = std::max(len_side2_num, r.length());
        len_side2_denom = std::max(len_side2_denom, Delta.length());

        // Simulation of Simplicity (symbolic perturbation)
        if(r_sign == ZERO) {
            cnt_side2_SOS++;
            const double* p_sort[3];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
	    
            SOS_sort(p_sort, p_sort + 3, dim);
	    
            for(index_t i = 0; i < 3; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1 = expansion_diff(Delta, a21);
                    const expansion& z = expansion_sum(z1, a20);
                    Sign z_sign = z.sign();
                    len_side2_SOS = std::max(len_side2_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                }
                if(p_sort[i] == p1) {
                    const expansion& z = expansion_diff(a21, a20);
                    Sign z_sign = z.sign();
                    len_side2_SOS = std::max(len_side2_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                }
                if(p_sort[i] == p2) {
                    return NEGATIVE;
                }
            }
            geo_assert_not_reached;
        }

        return Sign(Delta_sign * r_sign);
    }

    Sign side2_3d_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1
    ) {
        Sign result = Sign(side2_3d_filter(p0, p1, p2, q0, q1));
        if(result == ZERO) {
            result = side2_exact_SOS(p0, p1, p2, q0, q1, 3);
        }
        return result;
    }

    Sign side2_4d_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1
    ) {
        Sign result = Sign(side2_4d_filter(p0, p1, p2, q0, q1));
        if(result == ZERO) {
            result = side2_exact_SOS(p0, p1, p2, q0, q1, 4);
        }
        return result;
    }

    Sign side2_6d_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1
    ) {
        Sign result = Sign(side2_6d_filter(p0, p1, p2, q0, q1));
        if(result == ZERO) {
            result = side2_exact_SOS(p0, p1, p2, q0, q1, 6);
        }
        return result;
    }

    Sign side2_7d_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1
    ) {
        Sign result = Sign(side2_7d_filter(p0, p1, p2, q0, q1));
        if(result == ZERO) {
            result = side2_exact_SOS(p0, p1, p2, q0, q1, 7);
        }
        return result;
    }

    Sign side2_8d_SOS(
        const double* p0, const double* p1, const double* p2,
        const double* q0, const double* q1
    ) {
        Sign result = Sign(side2_8d_filter(p0, p1, p2, q0, q1));
        if(result == ZERO) {
            result = side2_exact_SOS(p0, p1, p2, q0, q1, 8);
        }
        return result;
    }
    
    // ================= side3 =========================================

    Sign side3_exact_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2,
        coord_index_t dim
    ) {
        cnt_side3_exact++;

        const expansion& l1 = expansion_sq_dist(p1, p0, dim);
        const expansion& l2 = expansion_sq_dist(p2, p0, dim);
        const expansion& l3 = expansion_sq_dist(p3, p0, dim);

        const expansion& a10 = expansion_dot_at(p1, q0, p0, dim).scale_fast(2.0);
        const expansion& a11 = expansion_dot_at(p1, q1, p0, dim).scale_fast(2.0);
        const expansion& a12 = expansion_dot_at(p1, q2, p0, dim).scale_fast(2.0);
        const expansion& a20 = expansion_dot_at(p2, q0, p0, dim).scale_fast(2.0);
        const expansion& a21 = expansion_dot_at(p2, q1, p0, dim).scale_fast(2.0);
        const expansion& a22 = expansion_dot_at(p2, q2, p0, dim).scale_fast(2.0);

        const expansion& a30 = expansion_dot_at(p3, q0, p0, dim).scale_fast(2.0);
        const expansion& a31 = expansion_dot_at(p3, q1, p0, dim).scale_fast(2.0);
        const expansion& a32 = expansion_dot_at(p3, q2, p0, dim).scale_fast(2.0);

        // [ b00 b01 b02 ]           [  1   1   1  ]-1
        // [ b10 b11 b12 ] = Delta * [ a10 a11 a12 ]
        // [ b20 b21 b22 ]           [ a20 a21 a22 ]

        const expansion& b00 = expansion_det2x2(a11, a12, a21, a22);
        const expansion& b01 = expansion_diff(a21, a22);
        const expansion& b02 = expansion_diff(a12, a11);
        const expansion& b10 = expansion_det2x2(a12, a10, a22, a20);
        const expansion& b11 = expansion_diff(a22, a20);
        const expansion& b12 = expansion_diff(a10, a12);
        const expansion& b20 = expansion_det2x2(a10, a11, a20, a21);
        const expansion& b21 = expansion_diff(a20, a21);
        const expansion& b22 = expansion_diff(a11, a10);

        const expansion& Delta = expansion_sum3(b00, b10, b20);
        Sign Delta_sign = Delta.sign();
        // Should not occur with symbolic
        // perturbation done at previous steps.
        geo_assert(Delta_sign != ZERO);

        //       [ Lambda0 ]   [ b01 b02 ]   [ l1 ]   [ b00 ]
        // Delta [ Lambda1 ] = [ b11 b12 ] * [    ] + [ b10 ]
        //       [ Lambda2 ]   [ b21 b22 ]   [ l2 ]   [ b20 ]

        const expansion& b01_l1 = expansion_product(b01, l1);
        const expansion& b02_l2 = expansion_product(b02, l2);
        const expansion& DeltaLambda0 = expansion_sum3(b01_l1, b02_l2, b00);

        const expansion& b11_l1 = expansion_product(b11, l1);
        const expansion& b12_l2 = expansion_product(b12, l2);
        const expansion& DeltaLambda1 = expansion_sum3(b11_l1, b12_l2, b10);

        const expansion& b21_l1 = expansion_product(b21, l1);
        const expansion& b22_l2 = expansion_product(b22, l2);
        const expansion& DeltaLambda2 = expansion_sum3(b21_l1, b22_l2, b20);

        // r = Delta*l3-(a30*DeltaLambda0+a31*DeltaLambda1+a32*DeltaLambda2)

        const expansion& r0 = expansion_product(Delta, l3);
        const expansion& r1 = expansion_product(a30, DeltaLambda0).negate();
        const expansion& r2 = expansion_product(a31, DeltaLambda1).negate();
        const expansion& r3 = expansion_product(a32, DeltaLambda2).negate();
        const expansion& r = expansion_sum4(r0, r1, r2, r3);
        Sign r_sign = r.sign();

        // Statistics
        len_side3_num = std::max(len_side3_num, r.length());
        len_side3_denom = std::max(len_side3_denom, Delta.length());

        // Simulation of Simplicity (symbolic perturbation)
        if(r_sign == ZERO) {
            cnt_side3_SOS++;
            const double* p_sort[4];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;
            SOS_sort(p_sort, p_sort + 4, dim);
            for(index_t i = 0; i < 4; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1_0 = expansion_sum(b01, b02);
                    const expansion& z1 = expansion_product(a30, z1_0).negate();
                    const expansion& z2_0 = expansion_sum(b11, b12);
                    const expansion& z2 = expansion_product(a31, z2_0).negate();
                    const expansion& z3_0 = expansion_sum(b21, b22);
                    const expansion& z3 = expansion_product(a32, z3_0).negate();
                    const expansion& z = expansion_sum4(Delta, z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3_SOS = std::max(len_side3_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    const expansion& z1 = expansion_product(a30, b01);
                    const expansion& z2 = expansion_product(a31, b11);
                    const expansion& z3 = expansion_product(a32, b21);
                    const expansion& z = expansion_sum3(z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3_SOS = std::max(len_side3_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p2) {
                    const expansion& z1 = expansion_product(a30, b02);
                    const expansion& z2 = expansion_product(a31, b12);
                    const expansion& z3 = expansion_product(a32, b22);
                    const expansion& z = expansion_sum3(z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3_SOS = std::max(len_side3_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p3) {
                    return NEGATIVE;
                }
            }
            geo_assert_not_reached;
        }
        return Sign(Delta_sign * r_sign);
    }


    Sign side3h_exact_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        double h0, double h1, double h2, double h3,
        const double* q0, const double* q1, const double* q2
    ) {
        cnt_side3h_exact++;

        const expansion& l1 = expansion_diff(h1,h0);
        const expansion& l2 = expansion_diff(h2,h0);
        const expansion& l3 = expansion_diff(h3,h0);

        const expansion& a10 = expansion_dot_at(p1, q0, p0, 3).scale_fast(2.0);
        const expansion& a11 = expansion_dot_at(p1, q1, p0, 3).scale_fast(2.0);
        const expansion& a12 = expansion_dot_at(p1, q2, p0, 3).scale_fast(2.0);
        const expansion& a20 = expansion_dot_at(p2, q0, p0, 3).scale_fast(2.0);
        const expansion& a21 = expansion_dot_at(p2, q1, p0, 3).scale_fast(2.0);
        const expansion& a22 = expansion_dot_at(p2, q2, p0, 3).scale_fast(2.0);

        const expansion& a30 = expansion_dot_at(p3, q0, p0, 3).scale_fast(2.0);
        const expansion& a31 = expansion_dot_at(p3, q1, p0, 3).scale_fast(2.0);
        const expansion& a32 = expansion_dot_at(p3, q2, p0, 3).scale_fast(2.0);

        // [ b00 b01 b02 ]           [  1   1   1  ]-1
        // [ b10 b11 b12 ] = Delta * [ a10 a11 a12 ]
        // [ b20 b21 b22 ]           [ a20 a21 a22 ]

        const expansion& b00 = expansion_det2x2(a11, a12, a21, a22);
        const expansion& b01 = expansion_diff(a21, a22);
        const expansion& b02 = expansion_diff(a12, a11);
        const expansion& b10 = expansion_det2x2(a12, a10, a22, a20);
        const expansion& b11 = expansion_diff(a22, a20);
        const expansion& b12 = expansion_diff(a10, a12);
        const expansion& b20 = expansion_det2x2(a10, a11, a20, a21);
        const expansion& b21 = expansion_diff(a20, a21);
        const expansion& b22 = expansion_diff(a11, a10);

        const expansion& Delta = expansion_sum3(b00, b10, b20);
        Sign Delta_sign = Delta.sign();
        // Should not occur with symbolic
        // perturbation done at previous steps.
        geo_assert(Delta_sign != ZERO);

        //       [ Lambda0 ]   [ b01 b02 ]   [ l1 ]   [ b00 ]
        // Delta [ Lambda1 ] = [ b11 b12 ] * [    ] + [ b10 ]
        //       [ Lambda2 ]   [ b21 b22 ]   [ l2 ]   [ b20 ]

        const expansion& b01_l1 = expansion_product(b01, l1);
        const expansion& b02_l2 = expansion_product(b02, l2);
        const expansion& DeltaLambda0 = expansion_sum3(b01_l1, b02_l2, b00);

        const expansion& b11_l1 = expansion_product(b11, l1);
        const expansion& b12_l2 = expansion_product(b12, l2);
        const expansion& DeltaLambda1 = expansion_sum3(b11_l1, b12_l2, b10);

        const expansion& b21_l1 = expansion_product(b21, l1);
        const expansion& b22_l2 = expansion_product(b22, l2);
        const expansion& DeltaLambda2 = expansion_sum3(b21_l1, b22_l2, b20);

        // r = Delta*l3-(a30*DeltaLambda0+a31*DeltaLambda1+a32*DeltaLambda2)

        const expansion& r0 = expansion_product(Delta, l3);
        const expansion& r1 = expansion_product(a30, DeltaLambda0).negate();
        const expansion& r2 = expansion_product(a31, DeltaLambda1).negate();
        const expansion& r3 = expansion_product(a32, DeltaLambda2).negate();
        const expansion& r = expansion_sum4(r0, r1, r2, r3);
        Sign r_sign = r.sign();

        // Statistics
        len_side3h_num = std::max(len_side3h_num, r.length());
        len_side3h_denom = std::max(len_side3h_denom, Delta.length());

        // Simulation of Simplicity (symbolic perturbation)
        if(r_sign == ZERO) {
            cnt_side3h_SOS++;
            const double* p_sort[4];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;

	    SOS_sort(p_sort, p_sort + 4, 3);
            for(index_t i = 0; i < 4; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1_0 = expansion_sum(b01, b02);
                    const expansion& z1 = expansion_product(a30, z1_0).negate();
                    const expansion& z2_0 = expansion_sum(b11, b12);
                    const expansion& z2 = expansion_product(a31, z2_0).negate();
                    const expansion& z3_0 = expansion_sum(b21, b22);
                    const expansion& z3 = expansion_product(a32, z3_0).negate();
                    const expansion& z = expansion_sum4(Delta, z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3h_SOS = std::max(len_side3h_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    const expansion& z1 = expansion_product(a30, b01);
                    const expansion& z2 = expansion_product(a31, b11);
                    const expansion& z3 = expansion_product(a32, b21);
                    const expansion& z = expansion_sum3(z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3h_SOS = std::max(len_side3h_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p2) {
                    const expansion& z1 = expansion_product(a30, b02);
                    const expansion& z2 = expansion_product(a31, b12);
                    const expansion& z3 = expansion_product(a32, b22);
                    const expansion& z = expansion_sum3(z1, z2, z3);
                    Sign z_sign = z.sign();
                    len_side3h_SOS = std::max(len_side3h_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p3) {
                    return NEGATIVE;
                }
            }
            geo_assert_not_reached;
        }
        return Sign(Delta_sign * r_sign);
    }

    
    Sign side3_3d_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2
    ) {
        Sign result = Sign(side3_3d_filter(p0, p1, p2, p3, q0, q1, q2));
        if(result == ZERO) {
            result = side3_exact_SOS(p0, p1, p2, p3, q0, q1, q2, 3);
        }
        return result;
    }

    
    Sign side3_4d_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2
    ) {
        Sign result = Sign(side3_4d_filter(p0, p1, p2, p3, q0, q1, q2));
        if(result == ZERO) {
            result = side3_exact_SOS(p0, p1, p2, p3, q0, q1, q2, 4);
        }
        return result;
    }

    Sign side3_6d_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2
    ) {
        Sign result = Sign(side3_6d_filter(p0, p1, p2, p3, q0, q1, q2));
        if(result == ZERO) {
            result = side3_exact_SOS(p0, p1, p2, p3, q0, q1, q2, 6);
        }
        return result;
    }

    Sign side3_7d_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2
    ) {
        Sign result = Sign(side3_7d_filter(p0, p1, p2, p3, q0, q1, q2));
        if(result == ZERO) {
            result = side3_exact_SOS(p0, p1, p2, p3, q0, q1, q2, 7);
        }
        return result;
    }

    Sign side3_8d_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* q0, const double* q1, const double* q2
    ) {
        Sign result = Sign(side3_8d_filter(p0, p1, p2, p3, q0, q1, q2));
        if(result == ZERO) {
            result = side3_exact_SOS(p0, p1, p2, p3, q0, q1, q2, 8);
        }
        return result;
    }
    
    // ================= side4 =========================================

    Sign side4_3d_exact_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* p4, bool sos = true
    ) {
        cnt_side4_exact++;

        const expansion& a11 = expansion_diff(p1[0], p0[0]);
        const expansion& a12 = expansion_diff(p1[1], p0[1]);
        const expansion& a13 = expansion_diff(p1[2], p0[2]);
        const expansion& a14 = expansion_sq_dist(p1, p0, 3).negate();

        const expansion& a21 = expansion_diff(p2[0], p0[0]);
        const expansion& a22 = expansion_diff(p2[1], p0[1]);
        const expansion& a23 = expansion_diff(p2[2], p0[2]);
        const expansion& a24 = expansion_sq_dist(p2, p0, 3).negate();

        const expansion& a31 = expansion_diff(p3[0], p0[0]);
        const expansion& a32 = expansion_diff(p3[1], p0[1]);
        const expansion& a33 = expansion_diff(p3[2], p0[2]);
        const expansion& a34 = expansion_sq_dist(p3, p0, 3).negate();

        const expansion& a41 = expansion_diff(p4[0], p0[0]);
        const expansion& a42 = expansion_diff(p4[1], p0[1]);
        const expansion& a43 = expansion_diff(p4[2], p0[2]);
        const expansion& a44 = expansion_sq_dist(p4, p0, 3).negate();

        // This commented-out version does not reuse
        // the 2x2 minors. 
/*
        const expansion& Delta1 = expansion_det3x3(
            a21, a22, a23,
            a31, a32, a33,
            a41, a42, a43
        );
        const expansion& Delta2 = expansion_det3x3(
            a11, a12, a13,
            a31, a32, a33,
            a41, a42, a43
        );
        const expansion& Delta3 = expansion_det3x3(
            a11, a12, a13,
            a21, a22, a23,
            a41, a42, a43
        );
        const expansion& Delta4 = expansion_det3x3(
            a11, a12, a13,
            a21, a22, a23,
            a31, a32, a33
        );
*/

        // Optimized version that reuses the 2x2 minors

        const expansion& m12 = expansion_det2x2(a12,a13,a22,a23);
        const expansion& m13 = expansion_det2x2(a12,a13,a32,a33);
        const expansion& m14 = expansion_det2x2(a12,a13,a42,a43);
        const expansion& m23 = expansion_det2x2(a22,a23,a32,a33);
        const expansion& m24 = expansion_det2x2(a22,a23,a42,a43);
        const expansion& m34 = expansion_det2x2(a32,a33,a42,a43);


        const expansion& z11 = expansion_product(a21,m34);
        const expansion& z12 = expansion_product(a31,m24).negate();
        const expansion& z13 = expansion_product(a41,m23);
        const expansion& Delta1 = expansion_sum3(z11,z12,z13);

        const expansion& z21 = expansion_product(a11,m34);
        const expansion& z22 = expansion_product(a31,m14).negate();
        const expansion& z23 = expansion_product(a41,m13);
        const expansion& Delta2 = expansion_sum3(z21,z22,z23);

        const expansion& z31 = expansion_product(a11,m24);
        const expansion& z32 = expansion_product(a21,m14).negate();
        const expansion& z33 = expansion_product(a41,m12);
        const expansion& Delta3 = expansion_sum3(z31,z32,z33);

        const expansion& z41 = expansion_product(a11,m23);
        const expansion& z42 = expansion_product(a21,m13).negate();
        const expansion& z43 = expansion_product(a31,m12);
        const expansion& Delta4 = expansion_sum3(z41,z42,z43);


        Sign Delta4_sign = Delta4.sign();
        geo_assert(Delta4_sign != ZERO);

        const expansion& r_1 = expansion_product(Delta1, a14);
        const expansion& r_2 = expansion_product(Delta2, a24).negate();
        const expansion& r_3 = expansion_product(Delta3, a34);
        const expansion& r_4 = expansion_product(Delta4, a44).negate();
        const expansion& r = expansion_sum4(r_1, r_2, r_3, r_4);
        Sign r_sign = r.sign();

        // Statistics
        len_side4_num = std::max(len_side4_num, r.length());
        len_side4_denom = std::max(len_side4_denom, Delta1.length());

        // Simulation of Simplicity (symbolic perturbation)
        if(sos && r_sign == ZERO) {
            cnt_side4_SOS++;
            const double* p_sort[5];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;
            p_sort[4] = p4;
            SOS_sort(p_sort, p_sort + 5, 3);
            for(index_t i = 0; i < 5; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1 = expansion_diff(Delta2, Delta1);
                    const expansion& z2 = expansion_diff(Delta4, Delta3);
                    const expansion& z = expansion_sum(z1, z2);
                    Sign z_sign = z.sign();
                    len_side4_SOS = std::max(len_side4_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta4_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    Sign Delta1_sign = Delta1.sign();
                    if(Delta1_sign != ZERO) {
                        len_side4_SOS = std::max(len_side4_SOS, Delta1.length());
                        return Sign(Delta4_sign * Delta1_sign);
                    }
                } else if(p_sort[i] == p2) {
                    Sign Delta2_sign = Delta2.sign();
                    if(Delta2_sign != ZERO) {
                        len_side4_SOS = std::max(len_side4_SOS, Delta2.length());
                        return Sign(-Delta4_sign * Delta2_sign);
                    }
                } else if(p_sort[i] == p3) {
                    Sign Delta3_sign = Delta3.sign();
                    if(Delta3_sign != ZERO) {
                        len_side4_SOS = std::max(len_side4_SOS, Delta3.length());
                        return Sign(Delta4_sign * Delta3_sign);
                    }
                } else if(p_sort[i] == p4) {
                    return NEGATIVE;
                }
            }
        }
        return Sign(Delta4_sign * r_sign);
    }

    Sign side4_exact_SOS(
        const double* p0, const double* p1, const double* p2, const double* p3,
        const double* p4,
        const double* q0, const double* q1, const double* q2, const double* q3,
        coord_index_t dim
    ) {
        cnt_side4_exact++;

        const expansion& l1 = expansion_sq_dist(p1, p0, dim);
        const expansion& l2 = expansion_sq_dist(p2, p0, dim);
        const expansion& l3 = expansion_sq_dist(p3, p0, dim);
        const expansion& l4 = expansion_sq_dist(p4, p0, dim);

        const expansion& a10 = expansion_dot_at(p1, q0, p0, dim).scale_fast(2.0);
        const expansion& a11 = expansion_dot_at(p1, q1, p0, dim).scale_fast(2.0);
        const expansion& a12 = expansion_dot_at(p1, q2, p0, dim).scale_fast(2.0);
        const expansion& a13 = expansion_dot_at(p1, q3, p0, dim).scale_fast(2.0);

        const expansion& a20 = expansion_dot_at(p2, q0, p0, dim).scale_fast(2.0);
        const expansion& a21 = expansion_dot_at(p2, q1, p0, dim).scale_fast(2.0);
        const expansion& a22 = expansion_dot_at(p2, q2, p0, dim).scale_fast(2.0);
        const expansion& a23 = expansion_dot_at(p2, q3, p0, dim).scale_fast(2.0);

        const expansion& a30 = expansion_dot_at(p3, q0, p0, dim).scale_fast(2.0);
        const expansion& a31 = expansion_dot_at(p3, q1, p0, dim).scale_fast(2.0);
        const expansion& a32 = expansion_dot_at(p3, q2, p0, dim).scale_fast(2.0);
        const expansion& a33 = expansion_dot_at(p3, q3, p0, dim).scale_fast(2.0);

        const expansion& a40 = expansion_dot_at(p4, q0, p0, dim).scale_fast(2.0);
        const expansion& a41 = expansion_dot_at(p4, q1, p0, dim).scale_fast(2.0);
        const expansion& a42 = expansion_dot_at(p4, q2, p0, dim).scale_fast(2.0);
        const expansion& a43 = expansion_dot_at(p4, q3, p0, dim).scale_fast(2.0);

        // [ b00 b01 b02 b03 ]           [  1   1   1   1  ]-1
        // [ b10 b11 b12 b13 ]           [ a10 a11 a12 a13 ]
        // [ b20 b21 b22 b23 ] = Delta * [ a20 a21 a22 a23 ]
        // [ b30 b31 b32 b33 ]           [ a30 a31 a32 a33 ]

        // Note: we could probably reuse some of the co-factors
        // (but for now I'd rather keep this form that is easier to
        //  read ... and to debug if need be !)

        const expansion& b00 = expansion_det3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33);
        const expansion& b01 = expansion_det_111_2x3(a21, a22, a23, a31, a32, a33).negate();
        const expansion& b02 = expansion_det_111_2x3(a11, a12, a13, a31, a32, a33);
        const expansion& b03 = expansion_det_111_2x3(a11, a12, a13, a21, a22, a23).negate();

        const expansion& b10 = expansion_det3x3(a10, a12, a13, a20, a22, a23, a30, a32, a33).negate();
        const expansion& b11 = expansion_det_111_2x3(a20, a22, a23, a30, a32, a33);
        const expansion& b12 = expansion_det_111_2x3(a10, a12, a13, a30, a32, a33).negate();
        const expansion& b13 = expansion_det_111_2x3(a10, a12, a13, a20, a22, a23);

        const expansion& b20 = expansion_det3x3(a10, a11, a13, a20, a21, a23, a30, a31, a33);
        const expansion& b21 = expansion_det_111_2x3(a20, a21, a23, a30, a31, a33).negate();
        const expansion& b22 = expansion_det_111_2x3(a10, a11, a13, a30, a31, a33);
        const expansion& b23 = expansion_det_111_2x3(a10, a11, a13, a20, a21, a23).negate();

        const expansion& b30 = expansion_det3x3(a10, a11, a12, a20, a21, a22, a30, a31, a32).negate();
        const expansion& b31 = expansion_det_111_2x3(a20, a21, a22, a30, a31, a32);
        const expansion& b32 = expansion_det_111_2x3(a10, a11, a12, a30, a31, a32).negate();
        const expansion& b33 = expansion_det_111_2x3(a10, a11, a12, a20, a21, a22);

        const expansion& Delta = expansion_sum4(b00, b10, b20, b30);
        Sign Delta_sign = Delta.sign();
        geo_assert(Delta_sign != ZERO);

        //       [ Lambda0 ]   [ b01 b02 b03 ]   [ l1 ]   [ b00 ]
        //       [ Lambda1 ]   [ b11 b12 b13 ]   [ l2 ]   [ b10 ]
        // Delta [ Lambda2 ] = [ b21 b22 b23 ] * [ l3 ] + [ b20 ]
        //       [ Lambda3 ]   [ b31 b32 b33 ]   [ l4 ]   [ b30 ]

        const expansion& b01_l1 = expansion_product(b01, l1);
        const expansion& b02_l2 = expansion_product(b02, l2);
        const expansion& b03_l3 = expansion_product(b03, l3);
        const expansion& DeltaLambda0 = expansion_sum4(b01_l1, b02_l2, b03_l3, b00);

        const expansion& b11_l1 = expansion_product(b11, l1);
        const expansion& b12_l2 = expansion_product(b12, l2);
        const expansion& b13_l3 = expansion_product(b13, l3);
        const expansion& DeltaLambda1 = expansion_sum4(b11_l1, b12_l2, b13_l3, b10);

        const expansion& b21_l1 = expansion_product(b21, l1);
        const expansion& b22_l2 = expansion_product(b22, l2);
        const expansion& b23_l3 = expansion_product(b23, l3);
        const expansion& DeltaLambda2 = expansion_sum4(b21_l1, b22_l2, b23_l3, b20);

        const expansion& b31_l1 = expansion_product(b31, l1);
        const expansion& b32_l2 = expansion_product(b32, l2);
        const expansion& b33_l3 = expansion_product(b33, l3);
        const expansion& DeltaLambda3 = expansion_sum4(b31_l1, b32_l2, b33_l3, b30);

        // r = Delta*l4 - (
        //    a40*DeltaLambda0+
        //    a41*DeltaLambda1+
        //    a42*DeltaLambda2+
        //    a43*DeltaLambda3
        // )

        const expansion& r0 = expansion_product(Delta, l4);
        const expansion& r1 = expansion_product(a40, DeltaLambda0);
        const expansion& r2 = expansion_product(a41, DeltaLambda1);
        const expansion& r3 = expansion_product(a42, DeltaLambda2);
        const expansion& r4 = expansion_product(a43, DeltaLambda3);
        const expansion& r1234 = expansion_sum4(r1, r2, r3, r4);
        const expansion& r = expansion_diff(r0, r1234);
        Sign r_sign = r.sign();

        // Simulation of Simplicity (symbolic perturbation)
        if(r_sign == ZERO) {
            cnt_side4_SOS++;
            const double* p_sort[5];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;
            p_sort[4] = p4;
            SOS_sort(p_sort, p_sort + 5, dim);
            for(index_t i = 0; i < 5; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1_0 = expansion_sum3(b01, b02, b03);
                    const expansion& z1 = expansion_product(a30, z1_0);
                    const expansion& z2_0 = expansion_sum3(b11, b12, b13);
                    const expansion& z2 = expansion_product(a31, z2_0);
                    const expansion& z3_0 = expansion_sum3(b21, b22, b23);
                    const expansion& z3 = expansion_product(a32, z3_0);
                    const expansion& z4_0 = expansion_sum3(b31, b32, b33);
                    const expansion& z4 = expansion_product(a33, z4_0);
                    const expansion& z1234 = expansion_sum4(z1, z2, z3, z4);
                    const expansion& z = expansion_diff(Delta, z1234);
                    Sign z_sign = z.sign();
                    len_side4_SOS = std::max(len_side4_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    const expansion& z1 = expansion_product(a30, b01);
                    const expansion& z2 = expansion_product(a31, b11);
                    const expansion& z3 = expansion_product(a32, b21);
                    const expansion& z4 = expansion_product(a33, b31);
                    const expansion& z = expansion_sum4(z1, z2, z3, z4);
                    Sign z_sign = z.sign();
                    len_side4_SOS = std::max(len_side4_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p2) {
                    const expansion& z1 = expansion_product(a30, b02);
                    const expansion& z2 = expansion_product(a31, b12);
                    const expansion& z3 = expansion_product(a32, b22);
                    const expansion& z4 = expansion_product(a33, b32);
                    const expansion& z = expansion_sum4(z1, z2, z3, z4);
                    Sign z_sign = z.sign();
                    len_side4_SOS = std::max(len_side4_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p3) {
                    const expansion& z1 = expansion_product(a30, b03);
                    const expansion& z2 = expansion_product(a31, b13);
                    const expansion& z3 = expansion_product(a32, b23);
                    const expansion& z4 = expansion_product(a33, b33);
                    const expansion& z = expansion_sum4(z1, z2, z3, z4);
                    Sign z_sign = z.sign();
                    len_side4_SOS = std::max(len_side4_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta_sign * z_sign);
                    }
                } else if(p_sort[i] == p4) {
                    return NEGATIVE;
                }
            }
            geo_assert_not_reached;
        }
        return Sign(r_sign * Delta_sign);
    }

    Sign side4_4d_SOS(
        const double* p0,
        const double* p1, const double* p2, const double* p3, const double* p4,
        const double* q0, const double* q1, const double* q2, const double* q3
    ) {
        Sign result = Sign(side4_4d_filter(p0, p1, p2, p3, p4, q0, q1, q2, q3));
        if(result == ZERO) {
            result = side4_exact_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3, 4);
        }
        return result;
    }

    Sign side4_6d_SOS(
        const double* p0,
        const double* p1, const double* p2, const double* p3, const double* p4,
        const double* q0, const double* q1, const double* q2, const double* q3
    ) {
        Sign result = Sign(side4_6d_filter(p0, p1, p2, p3, p4, q0, q1, q2, q3));
        if(result == ZERO) {
            result = side4_exact_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3, 6);
        }
        return result;
    }

    Sign side4_7d_SOS(
        const double* p0,
        const double* p1, const double* p2, const double* p3, const double* p4,
        const double* q0, const double* q1, const double* q2, const double* q3
    ) {
        Sign result = Sign(side4_7d_filter(p0, p1, p2, p3, p4, q0, q1, q2, q3));
        if(result == ZERO) {
            result = side4_exact_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3, 7);
        }
        return result;
    }

    Sign side4_8d_SOS(
        const double* p0,
        const double* p1, const double* p2, const double* p3, const double* p4,
        const double* q0, const double* q1, const double* q2, const double* q3
    ) {
        Sign result = Sign(side4_8d_filter(p0, p1, p2, p3, p4, q0, q1, q2, q3));
        if(result == ZERO) {
            result = side4_exact_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3, 8);
        }
        return result;
    }
    
    // ============ orient2d ==============================================

    Sign orient_2d_exact(
        const double* p0, const double* p1, const double* p2
    ) {
        cnt_orient2d_exact++;

        const expansion& a11 = expansion_diff(p1[0], p0[0]);
        const expansion& a12 = expansion_diff(p1[1], p0[1]);

        const expansion& a21 = expansion_diff(p2[0], p0[0]);
        const expansion& a22 = expansion_diff(p2[1], p0[1]);

        const expansion& Delta = expansion_det2x2(
            a11, a12, a21, a22
        );

        len_orient2d = std::max(len_orient2d, Delta.length());

        return Delta.sign();
    }


    // ============ orient3d ==============================================

    Sign orient_3d_exact(
        const double* p0, const double* p1,
        const double* p2, const double* p3
    ) {
        cnt_orient3d_exact++;

        const expansion& a11 = expansion_diff(p1[0], p0[0]);
        const expansion& a12 = expansion_diff(p1[1], p0[1]);
        const expansion& a13 = expansion_diff(p1[2], p0[2]);

        const expansion& a21 = expansion_diff(p2[0], p0[0]);
        const expansion& a22 = expansion_diff(p2[1], p0[1]);
        const expansion& a23 = expansion_diff(p2[2], p0[2]);

        const expansion& a31 = expansion_diff(p3[0], p0[0]);
        const expansion& a32 = expansion_diff(p3[1], p0[1]);
        const expansion& a33 = expansion_diff(p3[2], p0[2]);

        const expansion& Delta = expansion_det3x3(
            a11, a12, a13, a21, a22, a23, a31, a32, a33
        );

        len_orient3d = std::max(len_orient3d, Delta.length());

        return Delta.sign();
    }

    Sign side4h_3d_exact_SOS(
        const double* p0, const double* p1,
        const double* p2, const double* p3, const double* p4,
        double h0, double h1, double h2, double h3, double h4,
        bool sos = true
    ) {
        cnt_orient3dh_exact++;

        const expansion& a11 = expansion_diff(p1[0], p0[0]);
        const expansion& a12 = expansion_diff(p1[1], p0[1]);
        const expansion& a13 = expansion_diff(p1[2], p0[2]);
        const expansion& a14 = expansion_diff(h0,h1);

        const expansion& a21 = expansion_diff(p2[0], p0[0]);
        const expansion& a22 = expansion_diff(p2[1], p0[1]);
        const expansion& a23 = expansion_diff(p2[2], p0[2]);
        const expansion& a24 = expansion_diff(h0,h2);

        const expansion& a31 = expansion_diff(p3[0], p0[0]);
        const expansion& a32 = expansion_diff(p3[1], p0[1]);
        const expansion& a33 = expansion_diff(p3[2], p0[2]);
        const expansion& a34 = expansion_diff(h0,h3);

        const expansion& a41 = expansion_diff(p4[0], p0[0]);
        const expansion& a42 = expansion_diff(p4[1], p0[1]);
        const expansion& a43 = expansion_diff(p4[2], p0[2]);
        const expansion& a44 = expansion_diff(h0,h4);

        // Note: we could probably reuse some of the 2x2 co-factors
        // (but for now I'd rather keep this form that is easier to
        //  read ... and to debug if need be !)
        const expansion& Delta1 = expansion_det3x3(
            a21, a22, a23,
            a31, a32, a33,
            a41, a42, a43
        );
        const expansion& Delta2 = expansion_det3x3(
            a11, a12, a13,
            a31, a32, a33,
            a41, a42, a43
        );
        const expansion& Delta3 = expansion_det3x3(
            a11, a12, a13,
            a21, a22, a23,
            a41, a42, a43
        );
        const expansion& Delta4 = expansion_det3x3(
            a11, a12, a13,
            a21, a22, a23,
            a31, a32, a33
        );

        Sign Delta4_sign = Delta4.sign();
        geo_assert(Delta4_sign != ZERO);

        const expansion& r_1 = expansion_product(Delta1, a14);
        const expansion& r_2 = expansion_product(Delta2, a24).negate();
        const expansion& r_3 = expansion_product(Delta3, a34);
        const expansion& r_4 = expansion_product(Delta4, a44).negate();
        const expansion& r = expansion_sum4(r_1, r_2, r_3, r_4);

        Sign r_sign = r.sign();

        // Statistics
        len_orient3dh_num = std::max(len_orient3dh_num, r.length());
        len_orient3dh_denom = std::max(len_orient3dh_denom, Delta1.length());

        // Simulation of Simplicity (symbolic perturbation)
        if(sos && r_sign == ZERO) {
            cnt_orient3dh_SOS++;
            const double* p_sort[5];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;
            p_sort[4] = p4;

	    SOS_sort(p_sort, p_sort + 5, 3);
            for(index_t i = 0; i < 5; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1 = expansion_diff(Delta2, Delta1);
                    const expansion& z2 = expansion_diff(Delta4, Delta3);
                    const expansion& z = expansion_sum(z1, z2);
                    Sign z_sign = z.sign();
                    len_orient3dh_SOS = std::max(len_orient3dh_SOS, z.length());
                    if(z_sign != ZERO) {
                        return Sign(Delta4_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    Sign Delta1_sign = Delta1.sign();
                    if(Delta1_sign != ZERO) {
                        len_orient3dh_SOS = std::max(len_orient3dh_SOS, Delta1.length());
                        return Sign(Delta4_sign * Delta1_sign);
                    }
                } else if(p_sort[i] == p2) {
                    Sign Delta2_sign = Delta2.sign();
                    if(Delta2_sign != ZERO) {
                        len_orient3dh_SOS = std::max(len_orient3dh_SOS, Delta2.length());
                        return Sign(-Delta4_sign * Delta2_sign);
                    }
                } else if(p_sort[i] == p3) {
                    Sign Delta3_sign = Delta3.sign();
                    if(Delta3_sign != ZERO) {
                        len_orient3dh_SOS = std::max(len_orient3dh_SOS, Delta3.length());
                        return Sign(Delta4_sign * Delta3_sign);
                    }
                } else if(p_sort[i] == p4) {
                    return NEGATIVE;
                }
            }
        }
        return Sign(Delta4_sign * r_sign);
    }


    Sign side3h_2d_exact_SOS(
        const double* p0, const double* p1,
	const double* p2, const double* p3, 
        double h0, double h1, double h2, double h3,
        bool sos = true
    ) {

        const expansion& a11 = expansion_diff(p1[0], p0[0]);
        const expansion& a12 = expansion_diff(p1[1], p0[1]);
        const expansion& a13 = expansion_diff(h0,h1);

        const expansion& a21 = expansion_diff(p2[0], p0[0]);
        const expansion& a22 = expansion_diff(p2[1], p0[1]);
        const expansion& a23 = expansion_diff(h0,h2);

        const expansion& a31 = expansion_diff(p3[0], p0[0]);
        const expansion& a32 = expansion_diff(p3[1], p0[1]);
        const expansion& a33 = expansion_diff(h0,h3);

        const expansion& Delta1 = expansion_det2x2(
            a21, a22, 
            a31, a32
        );
        const expansion& Delta2 = expansion_det2x2(
            a11, a12,
            a31, a32
        );
        const expansion& Delta3 = expansion_det2x2(
            a11, a12,
            a21, a22
        );

        Sign Delta3_sign = Delta3.sign();
        geo_assert(Delta3_sign != ZERO);

        const expansion& r_1 = expansion_product(Delta1, a13);
        const expansion& r_2 = expansion_product(Delta2, a23).negate();
        const expansion& r_3 = expansion_product(Delta3, a33);
        const expansion& r = expansion_sum3(r_1, r_2, r_3);

        Sign r_sign = r.sign();

        // Simulation of Simplicity (symbolic perturbation)
        if(sos && r_sign == ZERO) {
            const double* p_sort[4];
            p_sort[0] = p0;
            p_sort[1] = p1;
            p_sort[2] = p2;
            p_sort[3] = p3;
            SOS_sort(p_sort, p_sort + 4, 3);
            for(index_t i = 0; i < 4; ++i) {
                if(p_sort[i] == p0) {
                    const expansion& z1 = expansion_diff(Delta2, Delta1);
                    const expansion& z = expansion_sum(z1, Delta3);
                    Sign z_sign = z.sign();
                    if(z_sign != ZERO) {
                        return Sign(Delta3_sign * z_sign);
                    }
                } else if(p_sort[i] == p1) {
                    Sign Delta1_sign = Delta1.sign();
                    if(Delta1_sign != ZERO) {
                        return Sign(Delta3_sign * Delta1_sign);
                    }
                } else if(p_sort[i] == p2) {
                    Sign Delta2_sign = Delta2.sign();
                    if(Delta2_sign != ZERO) {
                        return Sign(-Delta3_sign * Delta2_sign);
                    }
                } else if(p_sort[i] == p3) {
		    return NEGATIVE;
                } 
            }
        }
        return Sign(Delta3_sign * r_sign);
    }

    
    // ================================ det and dot =======================

    Sign det_3d_exact(
	const double* p0, const double* p1, const double* p2
    ) {
	const expansion& p0_0 = expansion_create(p0[0]);
	const expansion& p0_1 = expansion_create(p0[1]);
	const expansion& p0_2 = expansion_create(p0[2]);
	
	const expansion& p1_0 = expansion_create(p1[0]);
	const expansion& p1_1 = expansion_create(p1[1]);
	const expansion& p1_2 = expansion_create(p1[2]);

	const expansion& p2_0 = expansion_create(p2[0]);
	const expansion& p2_1 = expansion_create(p2[1]);
	const expansion& p2_2 = expansion_create(p2[2]);	
	
	const expansion& Delta = expansion_det3x3(
	    p0_0, p0_1, p0_2,
	    p1_0, p1_1, p1_2,
	    p2_0, p2_1, p2_2
	);
	return Delta.sign();
    }
    

    bool aligned_3d_exact(
	const double* p0, const double* p1, const double* p2
    ) {
	const expansion& U_0 = expansion_diff(p1[0],p0[0]);
	const expansion& U_1 = expansion_diff(p1[1],p0[1]);
	const expansion& U_2 = expansion_diff(p1[2],p0[2]);
	
	const expansion& V_0 = expansion_diff(p2[0],p0[0]);
	const expansion& V_1 = expansion_diff(p2[1],p0[1]);
	const expansion& V_2 = expansion_diff(p2[2],p0[2]);

	const expansion& N_0 = expansion_det2x2(U_1, V_1, U_2, V_2);
	const expansion& N_1 = expansion_det2x2(U_2, V_2, U_0, V_0);
	const expansion& N_2 = expansion_det2x2(U_0, V_0, U_1, V_1);

	return(
	    N_0.sign() == 0 &&
	    N_1.sign() == 0 &&
	    N_2.sign() == 0
	);
    }
    
    Sign dot_3d_exact(
	const double* p0, const double* p1, const double* p2
    ) {
	const expansion& U_0 = expansion_diff(p1[0],p0[0]);
	const expansion& U_1 = expansion_diff(p1[1],p0[1]);
	const expansion& U_2 = expansion_diff(p1[2],p0[2]);
	
	const expansion& V_0 = expansion_diff(p2[0],p0[0]);
	const expansion& V_1 = expansion_diff(p2[1],p0[1]);
	const expansion& V_2 = expansion_diff(p2[2],p0[2]);

	const expansion& UV_0 = expansion_product(U_0, V_0);
	const expansion& UV_1 = expansion_product(U_1, V_1);
	const expansion& UV_2 = expansion_product(U_2, V_2);

	const expansion& Delta = expansion_sum3(UV_0, UV_1, UV_2);

	return Delta.sign();
    }
    
    // ================================ statistics ========================

    inline double percent(index_t a, index_t b) {
        if(a == 0 && b == 0) {
            return 0;
        }
        return double(a) * 100.0 / double(b);
    }

    void show_stats_plain(
        const std::string& name, index_t cnt1, index_t cnt2
    ) {
        Logger::out(name)
            << "Tot:" << cnt1
            << " Exact:" << cnt2
            << std::endl;
        Logger::out(name)
            << " Exact: " << percent(cnt2, cnt1) << "% "
            << std::endl;
    }

    void show_stats_sos(
        const std::string& name, index_t cnt1, index_t cnt2, index_t cnt3
    ) {
        Logger::out(name)
            << "Tot:" << cnt1
            << " Exact:" << cnt2
            << " SOS:" << cnt3 << std::endl;
        Logger::out(name)
            << " Exact: " << percent(cnt2, cnt1) << "% "
            << " SOS: " << percent(cnt3, cnt1) << "% "
            << std::endl;
    }

    void show_stats_sos(
        const std::string& name, index_t cnt1, index_t cnt2, index_t cnt3,
        index_t len
    ) {
        show_stats_sos(name, cnt1, cnt2, cnt3);
        Logger::out(name) << " Len: " << len << std::endl;
    }

    void show_stats_plain(
        const std::string& name, index_t cnt1, index_t cnt2,
        index_t len
    ) {
        show_stats_plain(name, cnt1, cnt2);
        Logger::out(name) << " Len: " << len << std::endl;
    }

    void show_stats_sos(
        const std::string& name, index_t cnt1, index_t cnt2, index_t cnt3,
        index_t num_len, index_t denom_len, index_t SOS_len
    ) {
        show_stats_sos(name, cnt1, cnt2, cnt3);
        Logger::out(name)
            << " Num len: " << num_len
            << " Denom len: " << denom_len
            << " SOS len: " << SOS_len
            << std::endl;
    }
}



namespace GEO {

    namespace PCK {

	void set_SOS_mode(SOSMode m) {
	    SOS_mode_ = m;
	}

	SOSMode get_SOS_mode() {
	    return SOS_mode_;
	}

	
        Sign side1_SOS(
            const double* p0, const double* p1,
            const double* q0,
            coord_index_t DIM
        ) {
            cnt_side1_total++;
            switch(DIM) {
            case 3:
                return side1_3d_SOS(p0, p1, q0);
            case 4:
                return side1_4d_SOS(p0, p1, q0);
            case 6:
                return side1_6d_SOS(p0, p1, q0);
            case 7:
                return side1_7d_SOS(p0, p1, q0);
            case 8:
                return side1_8d_SOS(p0, p1, q0);
            }
            geo_assert_not_reached;
        }

        Sign side2_SOS(
            const double* p0, const double* p1, const double* p2,
            const double* q0, const double* q1,
            coord_index_t DIM
        ) {
            cnt_side2_total++;
            switch(DIM) {
            case 3:
                return side2_3d_SOS(p0, p1, p2, q0, q1);
            case 4:
                return side2_4d_SOS(p0, p1, p2, q0, q1);
            case 6:
                return side2_6d_SOS(p0, p1, p2, q0, q1);
            case 7:
                return side2_7d_SOS(p0, p1, p2, q0, q1);
            case 8:
                return side2_8d_SOS(p0, p1, p2, q0, q1);
            }
            geo_assert_not_reached;
        }

        Sign side3_SOS(
            const double* p0, const double* p1, const double* p2, const double* p3,
            const double* q0, const double* q1, const double* q2,
            coord_index_t DIM
        ) {
            cnt_side3_total++;
            switch(DIM) {
            case 3:
                return side3_3d_SOS(p0, p1, p2, p3, q0, q1, q2);
            case 4:
                return side3_4d_SOS(p0, p1, p2, p3, q0, q1, q2);
            case 6:
                return side3_6d_SOS(p0, p1, p2, p3, q0, q1, q2);
            case 7:
                return side3_7d_SOS(p0, p1, p2, p3, q0, q1, q2);
            case 8:
                return side3_8d_SOS(p0, p1, p2, p3, q0, q1, q2);
            }
            geo_assert_not_reached;
        }


        Sign side3_3dlifted_SOS(
            const double* p0, const double* p1, 
            const double* p2, const double* p3,
            double h0, double h1, double h2, double h3,            
            const double* q0, const double* q1, const double* q2,
	    bool SOS
        ) {
            Sign result = Sign(side3h_3d_filter(p0, p1, p2, p3, h0, h1, h2, h3, q0, q1, q2));
            if(SOS && result == ZERO) {
                result = side3h_exact_SOS(p0, p1, p2, p3, h0, h1, h2, h3, q0, q1, q2);
            }
            return result;
        }
        
        Sign side4_SOS(
            const double* p0,
            const double* p1, const double* p2, const double* p3, const double* p4,
            const double* q0, const double* q1, const double* q2, const double* q3,
            coord_index_t DIM
        ) {
            switch(DIM) {
            case 3:
                // 3d is a special case for side4()
                //   (intrinsic dim == ambient dim)
                // therefore embedding tet q0,q1,q2,q3 is not needed.
                // WARNING: cnt_side4_total is not incremented here,
                // because it is
                // incremented in side4_3d_SOS().
                return side4_3d_SOS(p0, p1, p2, p3, p4);
            case 4:
                cnt_side4_total++;
                return side4_4d_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3);
            case 6:
                cnt_side4_total++;
                return side4_6d_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3);
            case 7:
                cnt_side4_total++;
                return side4_7d_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3);
            case 8:
                cnt_side4_total++;
                return side4_8d_SOS(p0, p1, p2, p3, p4, q0, q1, q2, q3);
            }
            geo_assert_not_reached;
        }


        Sign side4_3d(
            const double* p0, const double* p1, const double* p2, const double* p3,
            const double* p4
        ) {
            cnt_side4_total++;
            Sign result = Sign(side4_3d_filter(p0, p1, p2, p3, p4));
            if(result == 0) {
                // last argument is false: do not apply symbolic perturbation
                result = side4_3d_exact_SOS(p0, p1, p2, p3, p4, false);
            }
            return result;
        }

        Sign side4_3d_SOS(
            const double* p0, const double* p1, 
            const double* p2, const double* p3,
            const double* p4
        ) {
            cnt_side4_total++;
            Sign result = Sign(side4_3d_filter(p0, p1, p2, p3, p4));
            if(result == 0) {
                result = side4_3d_exact_SOS(p0, p1, p2, p3, p4);
            }
            return result;
        }


        Sign in_sphere_3d_SOS(
            const double* p0, const double* p1, 
            const double* p2, const double* p3,
            const double* p4
        ) {
            // in_sphere_3d is simply implemented using side4_3d.
            // Both predicates are equivalent through duality as can
            // be easily seen:
            // side4_3d(p0,p1,p2,p3,p4) returns POSITIVE if
            //    d(q,p0) < d(q,p4)
            //    where q denotes the circumcenter of (p0,p1,p2,p3)
            // Note that d(q,p0) = R  (radius of circumscribed sphere)
            // In other words, side4_3d(p0,p1,p2,p3,p4) returns POSITIVE if
            //   d(q,p4) > R which means whenever p4 is not in the
            //   circumscribed sphere of (p0,p1,p2,p3).
            // Therefore:
            // in_sphere_3d(p0,p1,p2,p3,p4) = -side4_3d(p0,p1,p2,p3,p4)

            cnt_side4_total++;
            
            // This specialized filter supposes that orient_3d(p0,p1,p2,p3) > 0

#ifdef __AVX2__
	    Sign result = Sign(in_sphere_3d_filter_avx2(p0, p1, p2, p3, p4));
#else	    
            Sign result = Sign(in_sphere_3d_filter_optim(p0, p1, p2, p3, p4));
#endif	    
            if(result == 0) {
                result = side4_3d_exact_SOS(p0, p1, p2, p3, p4);
            }
            return Sign(-result);
        }

        Sign GEOGRAM_API in_circle_2d_SOS(
            const double* p0, const double* p1, const double* p2,
            const double* p3
        ) {
            // in_circle_2d is simply implemented using side3_2d.
            // Both predicates are equivalent through duality as can
            // be easily seen:
            // side3_2d(p0,p1,p2,p3,p0,p1,p2) returns POSITIVE if
            //    d(q,p0) < d(q,p3)
            //    where q denotes the circumcenter of (p0,p1,p2)
            // Note that d(q,p0) = R  (radius of circumscribed circle)
            // In other words, side3_2d(p0,p1,p2,p3,p4) returns POSITIVE if
            //   d(q,p3) > R which means whenever p3 is not in the
            //   circumscribed circle of (p0,p1,p2).
            // Therefore:
            // in_circle_2d(p0,p1,p2,p3) = -side3_2d(p0,p1,p2,p3)

	    // TODO: implement specialized filter like the one used
	    // by "in-sphere".
	    Sign s = Sign(-side3_2d_filter(p0, p1, p2, p3, p0, p1, p2));
	    if(s != ZERO) {
		return s;
	    }
	    return Sign(-side3_exact_SOS(p0, p1, p2, p3, p0, p1, p2, 2));
        }

        Sign GEOGRAM_API in_circle_3d_SOS(
            const double* p0, const double* p1, const double* p2,
            const double* p3
        ) {
            // in_circle_3d is simply implemented using side3_3d.
            // Both predicates are equivalent through duality as can
            // be easily seen:
            // side3_3d(p0,p1,p2,p3,p0,p1,p2) returns POSITIVE if
            //    d(q,p0) < d(q,p3)
            //    where q denotes the circumcenter of (p0,p1,p2)
            // Note that d(q,p0) = R  (radius of circumscribed circle)
            // In other words, side3_3d(p0,p1,p2,p3,p4) returns POSITIVE if
            //   d(q,p3) > R which means whenever p3 is not in the
            //   circumscribed circle of (p0,p1,p2).
            // Therefore:
            // in_circle_3d(p0,p1,p2,p3) = -side3_3d(p0,p1,p2,p3)
            return Sign(-side3_3d_SOS(p0,p1,p2,p3,p0,p1,p2));
        }

        Sign GEOGRAM_API in_circle_3dlifted_SOS(
            const double* p0, const double* p1, const double* p2,
            const double* p3,
            double h0, double h1, double h2, double h3,
	    bool SOS
        ) {
            // in_circle_3dlifted is simply implemented using side3_3dlifted.
            // Both predicates are equivalent through duality
            // (see comment in in_circle_3d_SOS(), the same
            //  remark applies).
            return Sign(-side3_3dlifted_SOS(p0,p1,p2,p3,h0,h1,h2,h3,p0,p1,p2,SOS));
        }

        
        Sign orient_2d(
            const double* p0, const double* p1, const double* p2
        ) {
            cnt_orient2d_total++;
            Sign result = Sign(orient_2d_filter(p0, p1, p2));
            if(result == 0) {
                result = orient_2d_exact(p0, p1, p2);
            }
            return result;
        }


        Sign orient_2dlifted_SOS(
            const double* p0, const double* p1,
            const double* p2, const double* p3, 
            double h0, double h1, double h2, double h3
	) {
            Sign result = Sign(
                side3_2dlifted_2d_filter(
                    p0, p1, p2, p3, h0, h1, h2, h3
                    )
                );
            if(result == 0) {
                result = side3h_2d_exact_SOS(
                    p0, p1, p2, p3, h0, h1, h2, h3
                );
            }
            // orient_3d() is opposite to side3h()
            // (like in_sphere() that is opposite to side3())
	    return result;
	}

	
        Sign orient_3d(
            const double* p0, const double* p1,
            const double* p2, const double* p3
            ) {
            cnt_orient3d_total++;
            Sign result = Sign(orient_3d_filter(p0, p1, p2, p3));
            if(result == 0) {
                result = orient_3d_exact(p0, p1, p2, p3);
            }
            return result;
        }


        Sign orient_3dlifted(
            const double* p0, const double* p1,
            const double* p2, const double* p3, const double* p4,
            double h0, double h1, double h2, double h3, double h4
        ) {
            cnt_orient3dh_total++;
            Sign result = Sign(
                side4h_3d_filter(
                    p0, p1, p2, p3, p4, h0, h1, h2, h3, h4
                    )
                );
            if(result == 0) {
                // last argument is false -> do not perturb.
                result = side4h_3d_exact_SOS(
                    p0, p1, p2, p3, p4, h0, h1, h2, h3, h4, false
                );
            }
            // orient_4d() is opposite to side4h()
            // (like in_sphere() that is opposite to side4())
            return Sign(-result);
        }

        Sign orient_3dlifted_SOS(
            const double* p0, const double* p1,
            const double* p2, const double* p3, const double* p4,
            double h0, double h1, double h2, double h3, double h4
        ) {
            cnt_orient3dh_total++;
            Sign result = Sign(
                side4h_3d_filter(
                    p0, p1, p2, p3, p4, h0, h1, h2, h3, h4
                    )
                );
            if(result == 0) {
                result = side4h_3d_exact_SOS(
                    p0, p1, p2, p3, p4, h0, h1, h2, h3, h4
                );
            }
            // orient_4d() is opposite to side4h()
            // (like in_sphere() that is opposite to side4())
            return Sign(-result);
        }

	Sign det_3d(
	    const double* p0, const double* p1, const double* p2
	) {
	    Sign result = Sign(
		det_3d_filter(p0, p1, p2)
	    );
	    if(result == 0) {
		result = det_3d_exact(p0, p1, p2);
	    }
	    return result;
	}


	Sign det_4d(
	    const double* p0, const double* p1, const double* p2, const double* p3
	) {
	    Sign result = Sign(
		det_4d_filter(p0, p1, p2, p3)
	    );
	    
	    if(result == 0) {
		const expansion& p0_0 = expansion_create(p0[0]);
		const expansion& p0_1 = expansion_create(p0[1]);
		const expansion& p0_2 = expansion_create(p0[2]);
		const expansion& p0_3 = expansion_create(p0[3]);		
		
		const expansion& p1_0 = expansion_create(p1[0]);
		const expansion& p1_1 = expansion_create(p1[1]);
		const expansion& p1_2 = expansion_create(p1[2]);
		const expansion& p1_3 = expansion_create(p1[3]);		
		
		const expansion& p2_0 = expansion_create(p2[0]);
		const expansion& p2_1 = expansion_create(p2[1]);
		const expansion& p2_2 = expansion_create(p2[2]);
		const expansion& p2_3 = expansion_create(p2[3]);

		const expansion& p3_0 = expansion_create(p3[0]);
		const expansion& p3_1 = expansion_create(p3[1]);
		const expansion& p3_2 = expansion_create(p3[2]);
		const expansion& p3_3 = expansion_create(p3[3]);			

		result = sign_of_expansion_determinant(
		    p0_0, p0_1, p0_2, p0_3,
		    p1_0, p1_1, p1_2, p1_3,
		    p2_0, p2_1, p2_2, p2_3,
		    p3_0, p3_1, p3_2, p3_3		    
		);
	    }
	    return result;
	}

	
	bool aligned_3d(
	    const double* p0, const double* p1, const double* p2
	) {
	    /*
	    Sign result = Sign(
		aligned_3d_filter(p0,p1,p2)
	    );
	    if(result != 0) {
		return false;
	    }
	    */
	    return aligned_3d_exact(p0, p1, p2);
	}
	
	Sign dot_3d(
	    const double* p0, const double* p1, const double* p2
	) {
	    Sign result = Sign(
		det_3d_filter(p0, p1, p2)
	    );
	    if(result == 0) {
		result = dot_3d_exact(p0, p1, p2);
	    }
	    return result;
	}

	bool points_are_identical_2d(
	    const double* p1,
	    const double* p2
	) {
	    return
		(p1[0] == p2[0]) &&
		(p1[1] == p2[1]) 
	    ;
	}

	bool points_are_identical_3d(
	    const double* p1,
	    const double* p2
	) {
	    return
		(p1[0] == p2[0]) &&
		(p1[1] == p2[1]) &&
		(p1[2] == p2[2])
	    ;
	}

	bool points_are_colinear_3d(
	    const double* p1,
	    const double* p2,
	    const double* p3
	) {
	    // Colinearity is tested by using four coplanarity
	    // tests with four points that are not coplanar.
	    // TODO: use PCK::aligned_3d() instead (to be tested)	
	    static const double q000[3] = {0.0, 0.0, 0.0};
	    static const double q001[3] = {0.0, 0.0, 1.0};
	    static const double q010[3] = {0.0, 1.0, 0.0};
	    static const double q100[3] = {1.0, 0.0, 0.0};
	    return
		PCK::orient_3d(p1, p2, p3, q000) == ZERO &&
		PCK::orient_3d(p1, p2, p3, q001) == ZERO &&
		PCK::orient_3d(p1, p2, p3, q010) == ZERO &&
		PCK::orient_3d(p1, p2, p3, q100) == ZERO
	    ;
	}
	
        void initialize() {
            expansion::initialize();
        }

        void terminate() {
            // Nothing to do.
        }

        void show_stats() {
            show_stats_plain(
                "orient2d",
                cnt_orient2d_total, cnt_orient2d_exact,
                len_orient2d
            );
            show_stats_plain(
                "orient3d",
                cnt_orient3d_total, cnt_orient3d_exact,
                len_orient3d
            );
            show_stats_sos(
                "orient3dh",
                cnt_orient3dh_total, cnt_orient3dh_exact, cnt_orient3dh_SOS,
                len_orient3dh_num, len_orient3dh_denom, len_orient3dh_SOS
            );
            show_stats_sos(
                "side1",
                cnt_side1_total, cnt_side1_exact, cnt_side1_SOS,
                len_side1
            );
            show_stats_sos(
                "side2",
                cnt_side2_total, cnt_side2_exact, cnt_side2_SOS,
                len_side2_num, len_side2_denom, len_side2_SOS
            );
            show_stats_sos(
                "side3",
                cnt_side3_total, cnt_side3_exact, cnt_side3_SOS,
                len_side3_num, len_side3_denom, len_side3_SOS
            );
            show_stats_sos(
                "side3h",
                cnt_side3h_total, cnt_side3h_exact, cnt_side3h_SOS,
                len_side3h_num, len_side3h_denom, len_side3h_SOS
            );
            show_stats_sos(
                "side4/insph.",
                cnt_side4_total, cnt_side4_exact, cnt_side4_SOS,
                len_side4_num, len_side4_denom, len_side4_SOS
            );
        }
    }
}


/******* extracted from delaunay_2d.h *******/

#ifndef GEOGRAM_DELAUNAY_DELAUNAY_2D
#define GEOGRAM_DELAUNAY_DELAUNAY_2D


#include <stack>


namespace GEO {

    class GEOGRAM_API Delaunay2d : public Delaunay {
    public:
        Delaunay2d(coord_index_t dimension = 2);

        virtual void set_vertices(
            index_t nb_vertices, const double* vertices
        );

        virtual index_t nearest_vertex(const double* p) const;


    protected:

        static const index_t NO_TRIANGLE = index_t(-1);

        bool create_first_triangle(
            index_t& iv0, index_t& iv1, index_t& iv2
        );

         index_t locate(
            const double* p, index_t hint = NO_TRIANGLE,
            bool thread_safe = false,
            Sign* orient = nullptr
         ) const;
         
         index_t locate_inexact(
             const double* p, index_t hint, index_t max_iter
         ) const;

         index_t insert(index_t v, index_t hint = NO_TRIANGLE);

         void find_conflict_zone(
             index_t v, 
             index_t t, const Sign* orient,
             index_t& t_bndry, index_t& e_bndry,
             index_t& first, index_t& last
         );
         
         void find_conflict_zone_iterative(
             const double* p, index_t t,
             index_t& t_bndry, index_t& e_bndry,
             index_t& first, index_t& last
         );

         
         index_t stellate_conflict_zone(
             index_t v, 
             index_t t_bndry, index_t e_bndry
         );
         
         // _________ Combinatorics - new and delete _________________________

        index_t max_t() const {
            return cell_to_v_store_.size() / 3;
        }


        static const index_t NOT_IN_LIST  = index_t(~0);

        static const index_t NOT_IN_LIST_BIT = index_t(1u << 31);

        static const index_t END_OF_LIST = ~(NOT_IN_LIST_BIT);


        bool triangle_is_in_list(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_next_[t] & NOT_IN_LIST_BIT) == 0;
        }

        index_t triangle_next(index_t t) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(triangle_is_in_list(t));
            return cell_next_[t];
        }

        void add_triangle_to_list(index_t t, index_t& first, index_t& last) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!triangle_is_in_list(t));
            if(last == END_OF_LIST) {
                geo_debug_assert(first == END_OF_LIST);
                first = last = t;
                cell_next_[t] = END_OF_LIST;
            } else {
                cell_next_[t] = first;
                first = t;
            }
        }

        void remove_triangle_from_list(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(triangle_is_in_list(t));
            cell_next_[t] = NOT_IN_LIST;
        }

        static const signed_index_t VERTEX_AT_INFINITY = -1;

        bool triangle_is_finite(index_t t) const {
            return 
                cell_to_v_store_[3 * t]     >= 0 &&
                cell_to_v_store_[3 * t + 1] >= 0 &&
                cell_to_v_store_[3 * t + 2] >= 0 ;
        }
        
        bool triangle_is_real(index_t t) const {
            return !triangle_is_free(t) && triangle_is_finite(t);
        }

        bool triangle_is_virtual(index_t t) const {
            return
            !triangle_is_free(t) && (
		cell_to_v_store_[3 * t] == VERTEX_AT_INFINITY ||
		cell_to_v_store_[3 * t + 1] == VERTEX_AT_INFINITY ||
		cell_to_v_store_[3 * t + 2] == VERTEX_AT_INFINITY
	    );
        }

        bool triangle_is_free(index_t t) const {
            return triangle_is_in_list(t);
        }

        index_t new_triangle() {
            index_t result;
            if(first_free_ == END_OF_LIST) {
                cell_to_v_store_.resize(cell_to_v_store_.size() + 3, -1);
                cell_to_cell_store_.resize(cell_to_cell_store_.size() + 3, -1);
                // index_t(NOT_IN_LIST) is necessary, else with
                // NOT_IN_LIST alone the compiler tries to generate a
                // reference to NOT_IN_LIST resulting in a link error.
                cell_next_.push_back(index_t(NOT_IN_LIST));
                result = max_t() - 1;
            } else {
                result = first_free_;
                first_free_ = triangle_next(first_free_);
                remove_triangle_from_list(result);
            }

            cell_to_cell_store_[3 * result] = -1;
            cell_to_cell_store_[3 * result + 1] = -1;
            cell_to_cell_store_[3 * result + 2] = -1;

            return result;
        }

        index_t new_triangle(
            signed_index_t v1, signed_index_t v2, 
            signed_index_t v3
        ) {
            index_t result = new_triangle();
            cell_to_v_store_[3 * result] = v1;
            cell_to_v_store_[3 * result + 1] = v2;
            cell_to_v_store_[3 * result + 2] = v3;
            return result;
        }

        void set_triangle_mark_stamp(index_t stamp) {
            cur_stamp_ = (stamp | NOT_IN_LIST_BIT);
        }

        bool triangle_is_marked(index_t t) const {
            return cell_next_[t] == cur_stamp_;
        }

        void mark_triangle(index_t t) {
            cell_next_[t] = cur_stamp_;
        }

        // _________ Combinatorics ___________________________________

        static index_t triangle_edge_vertex(index_t e, index_t v) {
            geo_debug_assert(e < 3);
            geo_debug_assert(v < 2);
            return index_t(triangle_edge_vertex_[e][v]);
        }

        signed_index_t triangle_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 3);
            return cell_to_v_store_[3 * t + lv];
        }

        index_t find_triangle_vertex(index_t t, signed_index_t v) const {
            geo_debug_assert(t < max_t());
            //   Find local index of v in triangle t vertices.
            const signed_index_t* T = &(cell_to_v_store_[3 * t]);
            return find_3(T,v);
        }


         index_t finite_triangle_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 3);
            geo_debug_assert(cell_to_v_store_[3 * t + lv] != -1);
            return index_t(cell_to_v_store_[3 * t + lv]);
        }

        void set_triangle_vertex(index_t t, index_t lv, signed_index_t v) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 3);
            cell_to_v_store_[3 * t + lv] = v;
        }

        signed_index_t triangle_adjacent(index_t t, index_t le) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(le < 3);
            signed_index_t result = cell_to_cell_store_[3 * t + le];
            return result;
        }

        void set_triangle_adjacent(index_t t1, index_t le1, index_t t2) {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2 < max_t());
            geo_debug_assert(le1 < 3);
	    geo_debug_assert(t1 != t2);
            cell_to_cell_store_[3 * t1 + le1] = signed_index_t(t2);
        }
        
        index_t find_triangle_adjacent(
            index_t t1, index_t t2_in
        ) const {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2_in < max_t());
            geo_debug_assert(t1 != t2_in);

            signed_index_t t2 = signed_index_t(t2_in);

            // Find local index of t2 in triangle t1 adajcent tets.
            const signed_index_t* T = &(cell_to_cell_store_[3 * t1]);
            index_t result = find_3(T,t2);

            // Sanity check: make sure that t1 is adjacent to t2
            // only once!
            geo_debug_assert(triangle_adjacent(t1,(result+1)%3) != t2);
            geo_debug_assert(triangle_adjacent(t1,(result+2)%3) != t2);
            return result;
        }



        void set_tet(
            index_t t,
            signed_index_t v0, signed_index_t v1,
            signed_index_t v2, 
            index_t a0, index_t a1, index_t a2
        ) {
            geo_debug_assert(t < max_t());
            cell_to_v_store_[3 * t] = v0;
            cell_to_v_store_[3 * t + 1] = v1;
            cell_to_v_store_[3 * t + 2] = v2;
            cell_to_cell_store_[3 * t] = signed_index_t(a0);
            cell_to_cell_store_[3 * t + 1] = signed_index_t(a1);
            cell_to_cell_store_[3 * t + 2] = signed_index_t(a2);
        }

        // _________ Predicates _____________________________________________

        bool triangle_is_conflict(index_t t, const double* p) const {

            // Lookup triangle vertices
            const double* pv[3];
            for(index_t i=0; i<3; ++i) {
                signed_index_t v = triangle_vertex(t,i);
                pv[i] = (v == -1) ? nullptr : vertex_ptr(index_t(v));
            }

            // Check for virtual triangles (then in_circle()
            // is replaced with orient2d())
            for(index_t le = 0; le < 3; ++le) {

                if(pv[le] == nullptr) {

                    // Facet of a virtual triangle opposite to
                    // infinite vertex corresponds to
                    // the triangle on the convex hull of the points.
                    // Orientation is obtained by replacing vertex lf
                    // with p.
                    pv[le] = p;
                    Sign sign = PCK::orient_2d(pv[0],pv[1],pv[2]);

                    if(sign > 0) {
                        return true;
                    }

                    if(sign < 0) {
                        return false;
                    }

                    // If sign is zero, we check the real triangle
                    // adjacent to the facet on the convex hull.
                    geo_debug_assert(triangle_adjacent(t, le) >= 0);
                    index_t t2 = index_t(triangle_adjacent(t, le));
                    geo_debug_assert(!triangle_is_virtual(t2));

                    //  If t2 is already chained in the conflict list,
                    // then it is conflict
                    if(triangle_is_in_list(t2)) {
                        return true;
                    }

                    //  If t2 is marked, then it is not in conflict.
                    if(triangle_is_marked(t2)) {
                        return false;
                    }

                    return triangle_is_conflict(t2, p);
                }
            }

            //   If the triangle is a finite one, it is in conflict
            // if its circumscribed sphere contains the point (this is
            // the standard case).

            if(weighted_) {
                double h0 = heights_[finite_triangle_vertex(t, 0)];
                double h1 = heights_[finite_triangle_vertex(t, 1)];
                double h2 = heights_[finite_triangle_vertex(t, 2)];
                index_t pindex = index_t(
                    (p - vertex_ptr(0)) / int(vertex_stride_)
                );
                double h = heights_[pindex];
                return (PCK::orient_2dlifted_SOS(
                            pv[0],pv[1],pv[2],p,h0,h1,h2,h
                       ) > 0) ;
            }

            return (PCK::in_circle_2d_SOS(pv[0], pv[1], pv[2], p) > 0);
        }

    protected:

        static index_t find_3(const signed_index_t* T, signed_index_t v) {
            // The following expression is 10% faster than using
            // if() statements. This uses the C++ norm, that 
            // ensures that the 'true' boolean value converted to 
            // an int is always 1. With most compilers, this avoids 
            // generating branching instructions.
            // Thank to Laurent Alonso for this idea.
            index_t result = index_t( (T[1] == v) | ((T[2] == v) * 2) );
            // Sanity check, important if it was T[0], not explicitly
            // tested (detects input that does not meet the precondition).
            geo_debug_assert(T[result] == v);
            return result; 
        }

        virtual ~Delaunay2d();

        void show_triangle(index_t t) const;

        void show_triangle_adjacent(index_t t, index_t le) const;

        void show_list(
            index_t first, const std::string& list_name
        ) const;

        void check_combinatorics(bool verbose = false) const;

        void check_geometry(bool verbose = false) const;

    private:
        vector<signed_index_t> cell_to_v_store_;
        vector<signed_index_t> cell_to_cell_store_;
        vector<index_t> cell_next_;
        vector<index_t> reorder_;
        index_t cur_stamp_; // used for marking
        index_t first_free_;
        bool weighted_;
        vector<double> heights_; // only used in weighted mode

         bool debug_mode_;

         bool verbose_debug_mode_;

         bool benchmark_mode_;

	 static char triangle_edge_vertex_[3][2];

	 std::stack<index_t> S_;
    };

    

    class GEOGRAM_API RegularWeightedDelaunay2d : public Delaunay2d {
    public:
        RegularWeightedDelaunay2d(coord_index_t dimension = 3);

    protected:
        virtual ~RegularWeightedDelaunay2d();
    };
}

#endif


/******* extracted from cavity.h *******/


#ifndef GEOGRAM_DELAUNAY_CAVITY
#define GEOGRAM_DELAUNAY_CAVITY

#include <string.h>

// Uncomment to display histogram of
// number of collisions per set() and
// get() operations.
// There is probably room for improvement
// in my hash function, but for large
// pointsets, more then 99% of queries are
// in the first slot (seems to be good enough).
//#define CAVITY_WITH_STATS
#ifdef CAVITY_WITH_STATS
#define CAVITY_STATS(x) x
#else
#define CAVITY_STATS(x)
#endif

namespace GEO {

    class Cavity {

      public:

	typedef Numeric::uint8 local_index_t;
	
	Cavity() {
	    clear();
#ifdef CAVITY_WITH_STATS	    
	    Memory::clear(stats_set_, sizeof(stats_set_));
	    Memory::clear(stats_get_, sizeof(stats_get_));
#endif	    
	}
	
	void clear() {
	    nb_f_ = 0;
	    OK_ = true;
	    ::memset(h2t_, END_OF_LIST, sizeof(h2t_));
	}

	~Cavity() {
#ifdef CAVITY_WITH_STATS	    
	    for(index_t i=0; i<MAX_H; ++i) {
		std::cerr << i << ": get=" << stats_get_[i] << "   set=" << stats_set_[i] << std::endl;
	    }
#endif	    
	}
	
	bool OK() const {
	    return OK_;
	}
	
	void new_facet(
	    index_t tglobal, index_t boundary_f,
	    signed_index_t v0, signed_index_t v1, signed_index_t v2
	) {
	    if(!OK_) {
		return;
	    }
	    
	    geo_debug_assert(v0 != v1);
	    geo_debug_assert(v1 != v2);
	    geo_debug_assert(v2 != v0);	    

	    local_index_t new_t = local_index_t(nb_f_);
	    
	    if(nb_f_ == MAX_F) {
		OK_ = false;
		return;
	    }
	    
	    set_vv2t(v0, v1, new_t);
	    set_vv2t(v1, v2, new_t);
	    set_vv2t(v2, v0, new_t);
	    
	    if(!OK_) {
		return;
	    }
	    
	    ++nb_f_;
	    tglobal_[new_t] = tglobal;
	    boundary_f_[new_t] = boundary_f;
	    f2v_[new_t][0] = v0;
	    f2v_[new_t][1] = v1;
	    f2v_[new_t][2] = v2;
	}

	index_t nb_facets() const {
	    return nb_f_;
	}

	index_t facet_tet(index_t f) const {
	    geo_debug_assert(f < nb_facets());
	    return tglobal_[f];
	}

	void set_facet_tet(index_t f, index_t t) {
	    geo_debug_assert(f < nb_facets());
	    tglobal_[f] = t;
	}

	index_t facet_facet(index_t f) const {
	    geo_debug_assert(f < nb_facets());
	    return boundary_f_[f];
	}

	signed_index_t facet_vertex(index_t f, index_t lv) const {
	    geo_debug_assert(f < nb_facets());
	    geo_debug_assert(lv < 3);
	    return f2v_[f][lv];
	}

	void get_facet_neighbor_tets(
	    index_t f, index_t& t0, index_t& t1, index_t& t2
	) const {
	    signed_index_t v0 = f2v_[f][0];
	    signed_index_t v1 = f2v_[f][1];
	    signed_index_t v2 = f2v_[f][2];		
	    t0 = tglobal_[get_vv2t(v2,v1)];
	    t1 = tglobal_[get_vv2t(v0,v2)];
	    t2 = tglobal_[get_vv2t(v1,v0)];
	}
	
      private:
	static const index_t        MAX_H = 1024; 
	static const local_index_t  END_OF_LIST = 255;
	static const index_t        MAX_F = 128;

	index_t hash(signed_index_t v1, signed_index_t v2) const {
	    return ((index_t(v1+1) ^ (419*index_t(v2+1))) % MAX_H);
	}

	void set_vv2t(
	    signed_index_t v1, signed_index_t v2, local_index_t f
	) {
	    CAVITY_STATS(index_t cnt = 0;)
	    index_t h = hash(v1,v2);
	    index_t cur = h;
	    do {
		if(h2t_[cur] == END_OF_LIST) {
		    h2t_[cur] = f;
#ifdef GARGANTUA		    
		    h2v_[cur][0] = v1;
		    h2v_[cur][1] = v2;
#else		    
		    h2v_[cur] = (Numeric::uint64(v1+1) << 32) | Numeric::uint64(v2+1);
#endif		    
		    CAVITY_STATS(++stats_set_[cnt];)
		    return;
		}
		cur = (cur+1)%MAX_H;
		CAVITY_STATS(++cnt;)
	    } while(cur != h);
	    OK_ = false;
	}

	local_index_t get_vv2t(signed_index_t v1, signed_index_t v2) const {
#ifndef GARGANTUA	    
	    Numeric::uint64 K = (Numeric::uint64(v1+1) << 32) | Numeric::uint64(v2+1);
#endif	    
	    CAVITY_STATS(index_t cnt = 0;)
	    index_t h = hash(v1,v2);
	    index_t cur = h;
	    do {
#ifdef GARGANTUA
		if((h2v_[cur][0] == v1) && (h2v_[cur][1] == v2)) {
#else
		if(h2v_[cur] == K) {
#endif		
	            CAVITY_STATS(++stats_get_[cnt];)
		    return h2t_[cur];
		}
		cur = (cur+1)%MAX_H;
		CAVITY_STATS(++cnt;)
	    } while(cur != h);
	    geo_assert_not_reached;
	}

	
	local_index_t  h2t_[MAX_H];

	
#ifdef GARGANTUA	
	signed_index_t h2v_[MAX_H][2];
#else	
	Numeric::uint64 h2v_[MAX_H];
#endif
	
	
	index_t nb_f_;
	
	
	index_t tglobal_[MAX_F];

	
	index_t boundary_f_[MAX_F];

	
	signed_index_t f2v_[MAX_F][3];
	
	
	bool OK_;

	CAVITY_STATS(mutable index_t stats_set_[MAX_H];)
	CAVITY_STATS(mutable index_t stats_get_[MAX_H];)
    };

}

#endif


/******* extracted from delaunay_3d.h *******/

#ifndef GEOGRAM_DELAUNAY_DELAUNAY_3D
#define GEOGRAM_DELAUNAY_DELAUNAY_3D


#include <stack>


namespace GEO {

    class GEOGRAM_API Delaunay3d : public Delaunay {
    public:
        Delaunay3d(coord_index_t dimension = 3);

        virtual void set_vertices(
            index_t nb_vertices, const double* vertices
        );

        virtual index_t nearest_vertex(const double* p) const;


    protected:

        static const index_t NO_TETRAHEDRON = index_t(-1);

        bool create_first_tetrahedron(
            index_t& iv0, index_t& iv1, index_t& iv2, index_t& iv3
        );

         index_t locate(
            const double* p, index_t hint = NO_TETRAHEDRON,
            bool thread_safe = false,
            Sign* orient = nullptr
         ) const;
         
         index_t locate_inexact(
             const double* p, index_t hint, index_t max_iter
         ) const;

         index_t insert(index_t v, index_t hint = NO_TETRAHEDRON);

         void find_conflict_zone(
             index_t v, 
             index_t t, const Sign* orient,
             index_t& t_bndry, index_t& f_bndry,
             index_t& first, index_t& last
         );
         
         void find_conflict_zone_iterative(
             const double* p, index_t t,
             index_t& t_bndry, index_t& f_bndry,
             index_t& first, index_t& last
         );

	 index_t stellate_cavity(index_t v);
	 
         
         index_t stellate_conflict_zone_iterative(
             index_t v, 
             index_t t_bndry, index_t f_bndry, 
             index_t prev_f=index_t(-1)
         );
         
         bool get_neighbor_along_conflict_zone_border(
             index_t t1,
             index_t t1fborder,
             index_t t1ft2,
             index_t& t2,
             index_t& t2fborder,
             index_t& t2ft1
         ) const {
             
             // Note: this function is a bit long for an inline function,
             // but I observed a (modest) performance gain doing so.
             
             //   Find two vertices that are both on facets new_f and f1
             //  (the edge around which we are turning)
             //  This uses duality as follows:
             //  Primal form (not used here): 
             //    halfedge_facet_[v1][v2] returns a facet that is incident
             //    to both v1 and v2.
             //  Dual form (used here):
             //    halfedge_facet_[f1][f2] returns a vertex that both 
             //    f1 and f2 are incident to.
             signed_index_t ev1 = 
                 tet_vertex(t1, index_t(halfedge_facet_[t1ft2][t1fborder]));
             signed_index_t ev2 = 
                 tet_vertex(t1, index_t(halfedge_facet_[t1fborder][t1ft2]));

             //   Turn around edge [ev1,ev2] inside the conflict zone
             // until we reach again the boundary of the conflict zone.
             // Traversing inside the conflict zone is faster (as compared
             // to outside) since it traverses a smaller number of tets.
             index_t cur_t = t1;
             index_t cur_f = t1ft2;
             index_t next_t = index_t(tet_adjacent(cur_t,cur_f));
             while(tet_is_in_list(next_t)) {
                 geo_debug_assert(next_t != t1);
                 cur_t = next_t;
                 cur_f = get_facet_by_halfedge(cur_t,ev1,ev2);
                 next_t = index_t(tet_adjacent(cur_t, cur_f));
             }
             
             //  At this point, cur_t is in conflict zone and
             // next_t is outside the conflict zone.
             index_t f12,f21;
             get_facets_by_halfedge(next_t, ev1, ev2, f12, f21);
             t2 = index_t(tet_adjacent(next_t,f21));
             signed_index_t v_neigh_opposite = tet_vertex(next_t,f12);
             t2ft1 = find_tet_vertex(t2, v_neigh_opposite);
             t2fborder = cur_f;
        
             //  Test whether the found neighboring tet was created
             //  (then return true) or is an old tet in conflict
             //  (then return false).
             return(t2 != cur_t);
         }
         
         // _________ Combinatorics - new and delete _________________________

        index_t max_t() const {
            return cell_to_v_store_.size() / 4;
        }


        static const index_t NOT_IN_LIST  = index_t(~0);

        static const index_t NOT_IN_LIST_BIT = index_t(1u << 31);

        static const index_t END_OF_LIST = ~(NOT_IN_LIST_BIT);


        bool tet_is_in_list(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_next_[t] & NOT_IN_LIST_BIT) == 0;
        }

        index_t tet_next(index_t t) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            return cell_next_[t];
        }

        void add_tet_to_list(index_t t, index_t& first, index_t& last) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!tet_is_in_list(t));
            if(last == END_OF_LIST) {
                geo_debug_assert(first == END_OF_LIST);
                first = last = t;
                cell_next_[t] = END_OF_LIST;
            } else {
                cell_next_[t] = first;
                first = t;
            }
        }

        void remove_tet_from_list(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            cell_next_[t] = NOT_IN_LIST;
        }

        static const signed_index_t VERTEX_AT_INFINITY = -1;

        bool tet_is_finite(index_t t) const {
            return 
                cell_to_v_store_[4 * t]     >= 0 &&
                cell_to_v_store_[4 * t + 1] >= 0 &&
                cell_to_v_store_[4 * t + 2] >= 0 &&
                cell_to_v_store_[4 * t + 3] >= 0;
        }
        
        bool tet_is_real(index_t t) const {
            return !tet_is_free(t) && tet_is_finite(t);
        }

        bool tet_is_virtual(index_t t) const {
            return
                !tet_is_free(t) && (
                cell_to_v_store_[4 * t] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 1] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 2] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 3] == VERTEX_AT_INFINITY) ;
        }

        bool tet_is_free(index_t t) const {
            return tet_is_in_list(t);
        }

        index_t new_tetrahedron() {
            index_t result;
            if(first_free_ == END_OF_LIST) {
                cell_to_v_store_.resize(cell_to_v_store_.size() + 4, -1);
                cell_to_cell_store_.resize(cell_to_cell_store_.size() + 4, -1);
                // index_t(NOT_IN_LIST) is necessary, else with
                // NOT_IN_LIST alone the compiler tries to generate a
                // reference to NOT_IN_LIST resulting in a link error.
                cell_next_.push_back(index_t(NOT_IN_LIST));
                result = max_t() - 1;
            } else {
                result = first_free_;
                first_free_ = tet_next(first_free_);
                remove_tet_from_list(result);
            }

            cell_to_cell_store_[4 * result] = -1;
            cell_to_cell_store_[4 * result + 1] = -1;
            cell_to_cell_store_[4 * result + 2] = -1;
            cell_to_cell_store_[4 * result + 3] = -1;

            return result;
        }

        index_t new_tetrahedron(
            signed_index_t v1, signed_index_t v2, 
            signed_index_t v3, signed_index_t v4
        ) {
            index_t result = new_tetrahedron();
            cell_to_v_store_[4 * result] = v1;
            cell_to_v_store_[4 * result + 1] = v2;
            cell_to_v_store_[4 * result + 2] = v3;
            cell_to_v_store_[4 * result + 3] = v4;
            return result;
        }

        void set_tet_mark_stamp(index_t stamp) {
            cur_stamp_ = (stamp | NOT_IN_LIST_BIT);
        }

        bool tet_is_marked(index_t t) const {
            return cell_next_[t] == cur_stamp_;
        }

        void mark_tet(index_t t) {
            cell_next_[t] = cur_stamp_;
        }

        // _________ Combinatorics ___________________________________

        static index_t tet_facet_vertex(index_t f, index_t v) {
            geo_debug_assert(f < 4);
            geo_debug_assert(v < 3);
            return index_t(tet_facet_vertex_[f][v]);
        }

        signed_index_t tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            return cell_to_v_store_[4 * t + lv];
        }

        index_t find_tet_vertex(index_t t, signed_index_t v) const {
            geo_debug_assert(t < max_t());
            //   Find local index of v in tetrahedron t vertices.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);
            return find_4(T,v);
        }


         index_t finite_tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            geo_debug_assert(cell_to_v_store_[4 * t + lv] != -1);
            return index_t(cell_to_v_store_[4 * t + lv]);
        }

        void set_tet_vertex(index_t t, index_t lv, signed_index_t v) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            cell_to_v_store_[4 * t + lv] = v;
        }

        signed_index_t tet_adjacent(index_t t, index_t lf) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lf < 4);
            signed_index_t result = cell_to_cell_store_[4 * t + lf];
            return result;
        }

        void set_tet_adjacent(index_t t1, index_t lf1, index_t t2) {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2 < max_t());
            geo_debug_assert(lf1 < 4);
            cell_to_cell_store_[4 * t1 + lf1] = signed_index_t(t2);
        }
        
        index_t find_tet_adjacent(
            index_t t1, index_t t2_in
        ) const {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2_in < max_t());
            geo_debug_assert(t1 != t2_in);

            signed_index_t t2 = signed_index_t(t2_in);

            // Find local index of t2 in tetrahedron t1 adajcent tets.
            const signed_index_t* T = &(cell_to_cell_store_[4 * t1]);
            index_t result = find_4(T,t2);

            // Sanity check: make sure that t1 is adjacent to t2
            // only once!
            geo_debug_assert(tet_adjacent(t1,(result+1)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+2)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+3)%4) != t2);
            return result;
        }



        void set_tet(
            index_t t,
            signed_index_t v0, signed_index_t v1,
            signed_index_t v2, signed_index_t v3,
            index_t a0, index_t a1, index_t a2, index_t a3
        ) {
            geo_debug_assert(t < max_t());
            cell_to_v_store_[4 * t] = v0;
            cell_to_v_store_[4 * t + 1] = v1;
            cell_to_v_store_[4 * t + 2] = v2;
            cell_to_v_store_[4 * t + 3] = v3;
            cell_to_cell_store_[4 * t] = signed_index_t(a0);
            cell_to_cell_store_[4 * t + 1] = signed_index_t(a1);
            cell_to_cell_store_[4 * t + 2] = signed_index_t(a2);
            cell_to_cell_store_[4 * t + 3] = signed_index_t(a3);
        }

        // _________ Combinatorics - traversals ______________________________

        index_t get_facet_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);
            //   Find local index of v1 and v2 in tetrahedron t
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);
            index_t lv1 = find_4(T,v1);
            index_t lv2 = find_4(T,v2);
            geo_debug_assert(lv1 != lv2);
            return index_t(halfedge_facet_[lv1][lv2]);
        }


        void get_facets_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2,
            index_t& f12, index_t& f21
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);

            //   Find local index of v1 and v2 in tetrahedron t
            // The following expression is 10% faster than using
            // if() statements (multiply by boolean result of test).
            // Thank to Laurent Alonso for this idea.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);

            signed_index_t lv1 = 
                (T[1] == v1) | ((T[2] == v1) * 2) | ((T[3] == v1) * 3);

            signed_index_t lv2 = 
                (T[1] == v2) | ((T[2] == v2) * 2) | ((T[3] == v2) * 3);

            geo_debug_assert(lv1 != 0 || T[0] == v1);
            geo_debug_assert(lv2 != 0 || T[0] == v2);
            geo_debug_assert(lv1 >= 0);
            geo_debug_assert(lv2 >= 0);
            geo_debug_assert(lv1 != lv2);

            f12 = index_t(halfedge_facet_[lv1][lv2]);
            f21 = index_t(halfedge_facet_[lv2][lv1]);
        }


        index_t next_around_halfedge(
            index_t& t, signed_index_t v1, signed_index_t v2
        ) const {
            return (index_t)tet_adjacent(
                t, get_facet_by_halfedge(t, v1, v2)
            );
        }

        // _________ Predicates _____________________________________________

        bool tet_is_conflict(index_t t, const double* p) const {

            // Lookup tetrahedron vertices
            const double* pv[4];
            for(index_t i=0; i<4; ++i) {
                signed_index_t v = tet_vertex(t,i);
                pv[i] = (v == -1) ? nullptr : vertex_ptr(index_t(v));
            }

            // Check for virtual tetrahedra (then in_sphere()
            // is replaced with orient3d())
            for(index_t lf = 0; lf < 4; ++lf) {

                if(pv[lf] == nullptr) {

                    // Facet of a virtual tetrahedron opposite to
                    // infinite vertex corresponds to
                    // the triangle on the convex hull of the points.
                    // Orientation is obtained by replacing vertex lf
                    // with p.
                    pv[lf] = p;
                    Sign sign = PCK::orient_3d(pv[0],pv[1],pv[2],pv[3]);

                    if(sign > 0) {
                        return true;
                    }

                    if(sign < 0) {
                        return false;
                    }

                    // If sign is zero, we check the real tetrahedron
                    // adjacent to the facet on the convex hull.
                    geo_debug_assert(tet_adjacent(t, lf) >= 0);
                    index_t t2 = index_t(tet_adjacent(t, lf));
                    geo_debug_assert(!tet_is_virtual(t2));

                    //  If t2 is already chained in the conflict list,
                    // then it is conflict
                    if(tet_is_in_list(t2)) {
                        return true;
                    }

                    //  If t2 is marked, then it is not in conflict.
                    if(tet_is_marked(t2)) {
                        return false;
                    }

                    return tet_is_conflict(t2, p);
                }
            }

            //   If the tetrahedron is a finite one, it is in conflict
            // if its circumscribed sphere contains the point (this is
            // the standard case).

            if(weighted_) {
                double h0 = heights_[finite_tet_vertex(t, 0)];
                double h1 = heights_[finite_tet_vertex(t, 1)];
                double h2 = heights_[finite_tet_vertex(t, 2)];
                double h3 = heights_[finite_tet_vertex(t, 3)];
                index_t pindex = index_t(
                    (p - vertex_ptr(0)) / int(vertex_stride_)
                );
                double h = heights_[pindex];
                return (PCK::orient_3dlifted_SOS(
                            pv[0],pv[1],pv[2],pv[3],p,h0,h1,h2,h3,h
                       ) > 0) ;
            }

            return (PCK::in_sphere_3d_SOS(pv[0], pv[1], pv[2], pv[3], p) > 0);
        }

    protected:

        static index_t find_4(const signed_index_t* T, signed_index_t v) {
            // The following expression is 10% faster than using
            // if() statements. This uses the C++ norm, that 
            // ensures that the 'true' boolean value converted to 
            // an int is always 1. With most compilers, this avoids 
            // generating branching instructions.
            // Thank to Laurent Alonso for this idea.
            // Note: Laurent also has this version:
            //    (T[0] != v)+(T[2]==v)+2*(T[3]==v)
            // that avoids a *3 multiply, but it is not faster in
            // practice.
            index_t result = index_t(
                (T[1] == v) | ((T[2] == v) * 2) | ((T[3] == v) * 3)
            );
            // Sanity check, important if it was T[0], not explicitly
            // tested (detects input that does not meet the precondition).
            geo_debug_assert(T[result] == v);
            return result; 
        }

        virtual ~Delaunay3d();

        void show_tet(index_t t) const;

        void show_tet_adjacent(index_t t, index_t lf) const;

        void show_list(
            index_t first, const std::string& list_name
        ) const;

        void check_combinatorics(bool verbose = false) const;

        void check_geometry(bool verbose = false) const;

    private:
        vector<signed_index_t> cell_to_v_store_;
        vector<signed_index_t> cell_to_cell_store_;
        vector<index_t> cell_next_;
        vector<index_t> reorder_;
        index_t cur_stamp_; // used for marking
        index_t first_free_;
        bool weighted_;
        vector<double> heights_; // only used in weighted mode

         bool debug_mode_;

         bool verbose_debug_mode_;

         bool benchmark_mode_;

        static char tet_facet_vertex_[4][3];

        static char halfedge_facet_[4][4];

        std::stack<index_t> S_;

        class StellateConflictStack {
        public:

            void push(index_t t1, index_t t1fbord, index_t t1fprev) {
                store_.resize(store_.size()+1);
                top().t1 = t1;
                top().t1fbord = Numeric::uint8(t1fbord);
                top().t1fprev = Numeric::uint8(t1fprev);
            }

            void save_locals(index_t new_t, index_t t1ft2, index_t t2ft1) {
                geo_debug_assert(!empty());
                top().new_t = new_t;
                top().t1ft2 = Numeric::uint8(t1ft2);
                top().t2ft1 = Numeric::uint8(t2ft1);
            }

            void get_parameters(
                index_t& t1, index_t& t1fbord, index_t& t1fprev
            ) const {
                geo_debug_assert(!empty());
                t1      = top().t1;
                t1fbord = index_t(top().t1fbord);
                t1fprev = index_t(top().t1fprev);
            }


            void get_locals(
                index_t& new_t, index_t& t1ft2, index_t& t2ft1
            ) const {
                geo_debug_assert(!empty());
                new_t = top().new_t;
                t1ft2 = index_t(top().t1ft2);
                t2ft1 = index_t(top().t2ft1);
            }
            
            void pop() {
                geo_debug_assert(!empty());
                store_.pop_back();
            }
            
            bool empty() const {
                return store_.empty();
            }
            
        private:

            struct Frame {
                // Parameters
                index_t t1;
                index_t new_t;                
                Numeric::uint8 t1fbord ;
                
                // Local variables
                Numeric::uint8 t1fprev ;
                Numeric::uint8 t1ft2   ;
                Numeric::uint8 t2ft1   ;
            };

            Frame& top() {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }

            const Frame& top() const {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }
            
            std::vector<Frame> store_;
        };

        StellateConflictStack S2_;

	Cavity cavity_;
    };

    

    class GEOGRAM_API RegularWeightedDelaunay3d : public Delaunay3d {
    public:
        RegularWeightedDelaunay3d(coord_index_t dimension = 4);

    protected:
        virtual ~RegularWeightedDelaunay3d();
    };
}

#endif


/******* extracted from parallel_delaunay_3d.h *******/

#ifndef GEOGRAM_PARALLEL_DELAUNAY_DELAUNAY_3D
#define GEOGRAM_PARALLEL_DELAUNAY_DELAUNAY_3D

#ifdef GEOGRAM_WITH_PDEL



namespace GEO {

     typedef Numeric::uint8 thread_index_t;
     
    class GEOGRAM_API ParallelDelaunay3d : public Delaunay {
    public:
        ParallelDelaunay3d(coord_index_t dimension = 3);

        virtual void set_vertices(
            index_t nb_vertices, const double* vertices
        );

        virtual index_t nearest_vertex(const double* p) const;

        virtual void set_BRIO_levels(const vector<index_t>& levels);

    private:
        vector<signed_index_t> cell_to_v_store_;
        vector<signed_index_t> cell_to_cell_store_;
        vector<index_t> cell_next_;
        vector<thread_index_t> cell_thread_;
        ThreadGroup threads_;
        bool weighted_; // true for regular triangulation.
        vector<double> heights_; // only used in weighted mode.        
        vector<index_t> reorder_;
        vector<index_t> levels_;

         bool debug_mode_;

         bool verbose_debug_mode_;

        bool benchmark_mode_;
        
        
        friend class Delaunay3dThread;
    };
    

}

#endif

#endif

/******* extracted from delaunay.cpp *******/


#ifdef GEOGRAM_WITH_PDEL
#endif

#ifdef GEOGRAM_WITH_TETGEN
#endif

#ifdef GEOGRAM_WITH_TRIANGLE
#endif


#include <fstream>
#include <sstream>

namespace {

    using namespace GEO;

    std::string invalid_dimension_error(
        coord_index_t dimension,
        const char* name,
        const char* expected
    ) {
        std::ostringstream out;
        out << "Invalid dimension: dimension " << index_t(dimension)
            << " is not supported by the " << name
            << " algorithm. Supported dimension(s): " << expected;
        return out.str();
    }
}

namespace GEO {

    Delaunay::InvalidDimension::InvalidDimension(
        coord_index_t dimension,
        const char* name,
        const char* expected
    ) :
        std::logic_error(invalid_dimension_error(dimension, name, expected)) {
    }

    const char* Delaunay::InvalidDimension::what() const GEO_NOEXCEPT {
        return std::logic_error::what();
    }


    Delaunay::InvalidInput::InvalidInput(int code) :
        logic_error("Invalid input for Delaunay"),
        error_code(code) {
    }

    Delaunay::InvalidInput::InvalidInput(
        const InvalidInput& rhs
    ) :
        std::logic_error(rhs),
        error_code(rhs.error_code),
        invalid_facets(rhs.invalid_facets) {
    }
    
    Delaunay::InvalidInput::~InvalidInput() GEO_NOEXCEPT {
    }
    
    const char* Delaunay::InvalidInput::what() const GEO_NOEXCEPT {
        return std::logic_error::what();
    }
    
    

    void Delaunay::initialize() {

#ifdef GEOGRAM_WITH_TETGEN
        geo_register_Delaunay_creator(DelaunayTetgen, "tetgen");
#endif

#ifdef GEOGRAM_WITH_TRIANGLE
        geo_register_Delaunay_creator(DelaunayTriangle, "triangle");
#endif
        
        geo_register_Delaunay_creator(Delaunay3d, "BDEL");

#ifdef GEOGRAM_WITH_PDEL
        geo_register_Delaunay_creator(ParallelDelaunay3d, "PDEL");
#endif
        geo_register_Delaunay_creator(RegularWeightedDelaunay3d, "BPOW");

	geo_register_Delaunay_creator(Delaunay2d, "BDEL2d");
	geo_register_Delaunay_creator(RegularWeightedDelaunay2d, "BPOW2d");
	
#ifndef GEOGRAM_PSM       
        geo_register_Delaunay_creator(Delaunay_NearestNeighbors, "NN");
#endif       
    }

    Delaunay* Delaunay::create(
        coord_index_t dim, const std::string& name_in
    ) {

        std::string name = name_in;
        if(name == "default") {
            name = CmdLine::get_arg("algo:delaunay");
        }

        try {
            Delaunay* d = DelaunayFactory::create_object(name, dim);
            if(d != nullptr) {
                return d;
            }

            Logger::warn("Delaunay")
                << "Could not create Delaunay triangulation: " << name
                << std::endl;
        }
        catch(InvalidDimension& ex) {
            Logger::warn("Delaunay") << ex.what() << std::endl;
        }

#ifdef GEOGRAM_PSM
       Logger::err("Delaunay")
            << "Could not create Delaunay triangulation"
            << std::endl;
       return nullptr;
#else       
        Logger::warn("Delaunay")
            << "Falling back to NN mode"
            << std::endl;

        return new Delaunay_NearestNeighbors(dim);
#endif       
    }

    Delaunay::Delaunay(coord_index_t dimension) {
        set_dimension(dimension);
        vertices_ = nullptr;
        nb_vertices_ = 0;
        nb_cells_ = 0;
        cell_to_v_ = nullptr;
        cell_to_cell_ = nullptr;
        is_locked_ = false;
        store_neighbors_ = false;
        default_nb_neighbors_ = 30;
        constraints_ = nullptr;
        do_reorder_ = true;
        refine_ = false;
        quality_ = 2.0;
        store_cicl_ = false;
        keep_infinite_ = false;
        nb_finite_cells_ = 0;
	keep_regions_ = false;
    }

    Delaunay::~Delaunay() {
    }

    void Delaunay::set_vertices(
        index_t nb_vertices, const double* vertices
    ) {
        nb_vertices_ = nb_vertices;
        vertices_ = vertices;
        if(nb_vertices_ < index_t(dimension()) + 1) {
            Logger::warn("Delaunay") << "Only "
                << nb_vertices
                << " vertices, may be not enough !"
                << std::endl;
        }
    }

    void Delaunay::set_BRIO_levels(const vector<index_t>& levels) {
        geo_argused(levels);
        // Default implementation does nothing
    }

    void Delaunay::set_arrays(
        index_t nb_cells,
        const signed_index_t* cell_to_v, const signed_index_t* cell_to_cell
    ) {
        nb_cells_ = nb_cells;
        cell_to_v_ = cell_to_v;
        cell_to_cell_ = cell_to_cell;

        if(cell_to_cell != nullptr) {
            if(store_cicl_) {
                update_v_to_cell();
                update_cicl();
            }
            if(store_neighbors_) {
                update_neighbors();
            }
        }
    }

    bool Delaunay::supports_constraints() const {
        return false;
    }

    index_t Delaunay::nearest_vertex(const double* p) const {
        // Unefficient implementation (but at least it works).
        // Derived classes are supposed to overload.
        geo_assert(nb_vertices() > 0);
        index_t result = 0;
        double d = Geom::distance2(vertex_ptr(0), p, dimension());
        for(index_t i = 1; i < nb_vertices(); i++) {
            double cur_d = Geom::distance2(vertex_ptr(i), p, dimension());
            if(cur_d < d) {
                d = cur_d;
                result = i;
            }
        }
        return result;
    }

    void Delaunay::update_neighbors() {
        if(nb_vertices() != neighbors_.nb_arrays()) {
            neighbors_.init(
                nb_vertices(),
                default_nb_neighbors_
            );
            for(index_t i = 0; i < nb_vertices(); i++) {
                neighbors_.resize_array(i, default_nb_neighbors_, false);
            }
        }
        parallel_for(
	    0, nb_vertices(),
	    [this](index_t i) { store_neighbors_CB(i); },
	    1, true
        );
    }

    void Delaunay::get_neighbors_internal(
        index_t v, vector<index_t>& neighbors
    ) const {
        // Step 1: traverse the incident cells list, and insert
        // all neighbors (may be duplicated)
        neighbors.resize(0);
        signed_index_t vt = v_to_cell_[v];
        if(vt != -1) { // Happens when there are duplicated vertices.
            index_t t = index_t(vt);
            do {
                index_t lvit = index(t, signed_index_t(v));
                // In the current cell, test all edges incident
                // to current vertex 'it'
                for(index_t lv = 0; lv < cell_size(); lv++) {
                    if(lvit != lv) {
                        signed_index_t neigh = cell_vertex(t, lv);
                        geo_debug_assert(neigh != -1);
                        neighbors.push_back(index_t(neigh));
                    }
                }
                t = index_t(next_around_vertex(t, index(t, signed_index_t(v))));
            } while(t != index_t(vt));
        }

        // Step 2: Sort the neighbors and remove all duplicates
        sort_unique(neighbors);
    }

    void Delaunay::store_neighbors_CB(index_t i) {
        // TODO: this one is not multithread-friendly
        // since it does dynamic memory allocation
        // (but not really a problem, since the one
        // that is used is in Delaunay_ANN).
        vector<index_t> neighbors;
        get_neighbors_internal(i, neighbors);
        neighbors_.set_array(i, neighbors);
    }

    void Delaunay::update_v_to_cell() {
        geo_assert(!is_locked_);  // Not thread-safe
        is_locked_ = true;

	// Note: if keeps_infinite is set, then infinite vertex
	// tet chaining is at t2v_[nb_vertices].
	
	if(keeps_infinite()) {	
	    v_to_cell_.assign(nb_vertices()+1, -1);
	    for(index_t c = 0; c < nb_cells(); c++) {
		for(index_t lv = 0; lv < cell_size(); lv++) {
		    signed_index_t v = cell_vertex(c, lv);
		    if(v == -1) {
			v = signed_index_t(nb_vertices());
		    }
		    v_to_cell_[v] = signed_index_t(c);
		}
	    }
	} else {
	    v_to_cell_.assign(nb_vertices(), -1);	    
	    for(index_t c = 0; c < nb_cells(); c++) {
		for(index_t lv = 0; lv < cell_size(); lv++) {
		    v_to_cell_[cell_vertex(c, lv)] = signed_index_t(c);
		}
	    }
	}
        is_locked_ = false;
    }

    void Delaunay::update_cicl() {
        geo_assert(!is_locked_);  // Not thread-safe
        is_locked_ = true;
        cicl_.resize(cell_size() * nb_cells());

	for(index_t v = 0; v < nb_vertices(); ++v) {
	    signed_index_t t = v_to_cell_[v];
	    if(t != -1) {
		index_t lv = index(index_t(t), signed_index_t(v));
		set_next_around_vertex(index_t(t), lv, index_t(t));
	    }
	}
	
	if(keeps_infinite()) {

	    {
		// Process the infinite vertex at index nb_vertices().
		signed_index_t t = v_to_cell_[nb_vertices()];
		if(t != -1) {
		    index_t lv = index(index_t(t), -1);
		    set_next_around_vertex(index_t(t), lv, index_t(t));
		}
	    }

	    for(index_t t = 0; t < nb_cells(); ++t) {
		for(index_t lv = 0; lv < cell_size(); ++lv) {
		    signed_index_t v = cell_vertex(t, lv);
		    index_t vv = (v == -1) ? nb_vertices() : index_t(v);
		    if(v_to_cell_[vv] != signed_index_t(t)) {
			index_t t1 = index_t(v_to_cell_[vv]);
			index_t lv1 = index(t1, signed_index_t(v));
			index_t t2 = index_t(next_around_vertex(t1, lv1));
			set_next_around_vertex(t1, lv1, t);
			set_next_around_vertex(t, lv, t2);
		    }
		}
	    }
	    
	    
	} else {
	    for(index_t t = 0; t < nb_cells(); ++t) {
		for(index_t lv = 0; lv < cell_size(); ++lv) {
		    index_t v = index_t(cell_vertex(t, lv));
		    if(v_to_cell_[v] != signed_index_t(t)) {
			index_t t1 = index_t(v_to_cell_[v]);
			index_t lv1 = index(t1, signed_index_t(v));
			index_t t2 = index_t(next_around_vertex(t1, lv1));
			set_next_around_vertex(t1, lv1, t);
			set_next_around_vertex(t, lv, t2);
		    }
		}
	    }
	}
	
        is_locked_ = false;
    }

    void Delaunay::save_histogram(std::ostream& out) const {
        vector<index_t> histogram;
        for(index_t v = 0; v < nb_vertices(); v++) {
            index_t N = neighbors_.array_size(v);
            if(histogram.size() < N) {
                histogram.resize(N + 1);
            }
            histogram[N]++;
        }
        for(index_t i = 0; i < histogram.size(); i++) {
            out << i << " " << histogram[i] << std::endl;
        }
    }

    bool Delaunay::cell_is_infinite(index_t c) const {
        geo_debug_assert(c < nb_cells());
        for(index_t lv=0; lv < cell_size(); ++lv) {
            if(cell_vertex(c,lv) == -1) {
                return true;
            }
        }
        return false;
    }

    index_t Delaunay::region(index_t t) const {
	geo_argused(t);
	geo_debug_assert(t < nb_cells());
	return index_t(-1);
    }
}


/******* extracted from delaunay_2d.cpp *******/


#include <stack>
#include <algorithm>

// TODO: optimizations:
// - convex hull traversal for nearest_vertex()

namespace {
    using namespace GEO;

    inline Sign orient_2d_inexact(
        const double* p0, const double* p1,
        const double* p2
    ) {
        double a11 = p1[0] - p0[0] ;
        double a12 = p1[1] - p0[1] ;
        
        double a21 = p2[0] - p0[0] ;
        double a22 = p2[1] - p0[1] ;
        
        double Delta = det2x2(
            a11,a12,
            a21,a22
        );

        return geo_sgn(Delta);
    }
}

namespace GEO {

    // triangle edge vertex is such that the triangle
    // formed with:
    //  vertex lv
    //  triangle_edge_vertex[lv][0]
    //  triangle_edge_vertex[lv][1]
    // has the same orientation as the original triangle for
    // any vertex lv.

    char Delaunay2d::triangle_edge_vertex_[3][2] = {
        {1,2},
        {2,0},
        {0,1}
    };

    Delaunay2d::Delaunay2d(coord_index_t dimension) :
        Delaunay(dimension)
    {
	geo_cite_with_info(
	    "DBLP:journals/cj/Bowyer81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
        );
	geo_cite_with_info(
	    "journals/cj/Watson81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
	);
	geo_cite_with_info(
	    "DBLP:conf/compgeom/AmentaCR03",
	    "Using spatial sorting has a dramatic impact on the performances."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/FunkeMN05",
	    "Initializing \\verb|locate()| with a non-exact version "
	    " (structural filtering) gains (a bit of) performance."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/BoissonnatDPTY02",
	    "The idea of traversing the cavity from inside "
	    " used in GEOGRAM is inspired by the implementation of "
	    " \\verb|Delaunay_triangulation_3| in CGAL."
	);
	geo_cite_with_info(
	    "DBLP:conf/imr/Si06",
	    "The triangulation data structure used in GEOGRAM is inspired "
	    "by Tetgen."
	);
	geo_cite_with_info(
	    "DBLP:journals/ijfcs/DevillersPT02",
	    "Analysis of the different versions of the line walk algorithm "
	    " used by \\verb|locate()|."
	);
	
        if(dimension != 2 && dimension != 3) {
            throw InvalidDimension(dimension, "Delaunay2d", "2 or 3");
        }
        first_free_ = END_OF_LIST;
        weighted_ = (dimension == 3);
        // In weighted mode, vertices are 3d but combinatorics is 2d.
        if(weighted_) {
            cell_size_ = 3;
            cell_v_stride_ = 3;
            cell_neigh_stride_ = 3;
        }
        cur_stamp_ = 0;
        debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay");
        verbose_debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay_verbose");
        debug_mode_ = (debug_mode_ || verbose_debug_mode_);
        benchmark_mode_ = CmdLine::get_arg_bool("dbg:delaunay_benchmark");
    }

    Delaunay2d::~Delaunay2d() {
    }

    void Delaunay2d::set_vertices(
        index_t nb_vertices, const double* vertices
    ) {
        Stopwatch* W = nullptr;
        if(benchmark_mode_) {
            W = new Stopwatch("DelInternal");
        }
        cur_stamp_ = 0;
        if(weighted_) {
            heights_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                // Client code uses 3d embedding with ti = sqrt(W - wi)
                //   where W = max(wi)
                // We recompute the standard "shifted" lifting on
                // the paraboloid from it.
                // (we use wi - W, everything is shifted by W, but
                // we do not care since the power diagram is invariant
                // by a translation of all weights).
                double w = -geo_sqr(vertices[3 * i + 2]);
                heights_[i] = -w +
                    geo_sqr(vertices[3 * i]) +
                    geo_sqr(vertices[3 * i + 1]); 
            }
        }

        Delaunay::set_vertices(nb_vertices, vertices);

        index_t expected_triangles = nb_vertices * 2;

        cell_to_v_store_.reserve(expected_triangles * 3);
        cell_to_cell_store_.reserve(expected_triangles * 3);
        cell_next_.reserve(expected_triangles);

        cell_to_v_store_.resize(0);
        cell_to_cell_store_.resize(0);
        cell_next_.resize(0);
        first_free_ = END_OF_LIST;

        //   Sort the vertices spatially. This makes localisation
        // faster.
        if(do_reorder_) {
            compute_BRIO_order(
                nb_vertices, vertex_ptr(0), reorder_, 2, dimension_
            );
	} else {
            reorder_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                reorder_[i] = i;
            }
        }

        double sorting_time = 0;
        if(benchmark_mode_) {
            sorting_time = W->elapsed_time();
            Logger::out("DelInternal1") << "BRIO sorting:"
                                       << sorting_time
                                       << std::endl;
        } 

        // The indices of the vertices of the first tetrahedron.
        index_t v0, v1, v2;
        if(!create_first_triangle(v0, v1, v2)) {
            Logger::warn("Delaunay2d") << "All the points are colinear"
                << std::endl;
            return;
        }

        index_t hint = NO_TRIANGLE;
        // Insert all the vertices incrementally.
        for(index_t i = 0; i < nb_vertices; ++i) {
            index_t v = reorder_[i];
            // Do not re-insert the first four vertices.
            if(v != v0 && v != v1 && v != v2) {
                index_t new_hint = insert(v, hint);
                if(new_hint != NO_TRIANGLE) {
                    hint = new_hint;
                }
            }
        }

        if(benchmark_mode_) {
            Logger::out("DelInternal2") << "Core insertion algo:"
                                       << W->elapsed_time() - sorting_time
                                       << std::endl;
        }
        delete W;

        if(debug_mode_) {
            check_combinatorics(verbose_debug_mode_);
            check_geometry(verbose_debug_mode_);
        }

        //   Compress cell_to_v_store_ and cell_to_cell_store_
        // (remove free and virtual tetrahedra).
        //   Since cell_next_ is not used at this point,
        // we reuse it for storing the conversion array that
        // maps old tet indices to new tet indices
        // Note: tet_is_real() uses the previous value of 
        // cell_next(), but we are processing indices
        // in increasing order and since old2new[t] is always
        // smaller or equal to t, we never overwrite a value
        // before needing it.
        
        vector<index_t>& old2new = cell_next_;
        index_t nb_triangles = 0;
        index_t nb_triangles_to_delete = 0;
        
        {
            for(index_t t = 0; t < max_t(); ++t) {
                if(
                    (keep_infinite_ && !triangle_is_free(t)) ||
                    triangle_is_real(t)
                ) {
                    if(t != nb_triangles) {
                        Memory::copy(
                            &cell_to_v_store_[nb_triangles * 3],
                            &cell_to_v_store_[t * 3],
                            3 * sizeof(signed_index_t)
                        );
                        Memory::copy(
                            &cell_to_cell_store_[nb_triangles * 3],
                            &cell_to_cell_store_[t * 3],
                            3 * sizeof(signed_index_t)
                        );
                    }
                    old2new[t] = nb_triangles;
                    ++nb_triangles;
                } else {
                    old2new[t] = index_t(-1);
                    ++nb_triangles_to_delete;
                }
            }
            cell_to_v_store_.resize(3 * nb_triangles);
            cell_to_cell_store_.resize(3 * nb_triangles);
            for(index_t i = 0; i < 3 * nb_triangles; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                // Note: t can be equal to -1 when a real tet is
                // adjacent to a virtual one (and this is how the
                // rest of Vorpaline expects to see tets on the
                // border).
                cell_to_cell_store_[i] = t;
            }
        }

        // In "keep_infinite" mode, we reorder the cells in such
        // a way that finite cells have indices [0..nb_finite_cells_-1]
        // and infinite cells have indices [nb_finite_cells_ .. nb_cells_-1]
        
        if(keep_infinite_) {
            nb_finite_cells_ = 0;
            index_t finite_ptr = 0;
            index_t infinite_ptr = nb_triangles - 1;
            for(;;) {
                while(triangle_is_finite(finite_ptr)) {
                    old2new[finite_ptr] = finite_ptr;
                    ++finite_ptr;
                    ++nb_finite_cells_;
                }
                while(!triangle_is_finite(infinite_ptr)) {
                    old2new[infinite_ptr] = infinite_ptr;
                    --infinite_ptr;
                }
                if(finite_ptr > infinite_ptr) {
                    break;
                }
                old2new[finite_ptr] = infinite_ptr;
                old2new[infinite_ptr] = finite_ptr;
                ++nb_finite_cells_;
                for(index_t lf=0; lf<3; ++lf) {
		    std::swap(
                        cell_to_cell_store_[3*finite_ptr + lf],
                        cell_to_cell_store_[3*infinite_ptr + lf]
                    );
                }
                for(index_t lv=0; lv<3; ++lv) {
		    std::swap(
                        cell_to_v_store_[3*finite_ptr + lv],
                        cell_to_v_store_[3*infinite_ptr + lv]
                    );
                }
                ++finite_ptr;
                --infinite_ptr;
            }
            for(index_t i = 0; i < 3 * nb_triangles; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                geo_debug_assert(t >= 0);
                cell_to_cell_store_[i] = t;
            }
        }

        if(benchmark_mode_) {
            if(keep_infinite_) {
                Logger::out("DelCompress") 
                    << "Removed " << nb_triangles_to_delete 
                    << " triangles (free list)" << std::endl;
            } else {
                Logger::out("DelCompress") 
                    << "Removed " << nb_triangles_to_delete 
                    << " triangles (free list and infinite)" << std::endl;
            }
        }
        
        set_arrays(
            nb_triangles,
            cell_to_v_store_.data(), cell_to_cell_store_.data()
        );
    }

    index_t Delaunay2d::nearest_vertex(const double* p) const {

        // TODO: For the moment, we fallback to the (unefficient)
        // baseclass implementation when in weighted mode.
        if(weighted_) {
            return Delaunay::nearest_vertex(p);
        }

        // Find a triangle (real or virtual) that contains p
        index_t t = locate(p, NO_TRIANGLE, thread_safe());

        //   If p is outside the convex hull of the inserted points,
        // a special traversal is required (not implemented yet).
        // TODO: implement convex hull boundary traversal
        // (for now we fallback to linear search implemented
        //  in baseclass)
        if(t == NO_TRIANGLE || triangle_is_virtual(t)) {
            return Delaunay::nearest_vertex(p);
        }

        double sq_dist = 1e30;
        index_t result = NO_TRIANGLE;

        // Find the nearest vertex among t's vertices
        for(index_t lv = 0; lv < 3; ++lv) {
            signed_index_t v = triangle_vertex(t, lv);
            // If the tetrahedron is virtual, then the first vertex
            // is the vertex at infinity and is skipped.
            if(v < 0) {
                continue;
            }
            double cur_sq_dist = Geom::distance2(p, vertex_ptr(index_t(v)), 2);
            if(cur_sq_dist < sq_dist) {
                sq_dist = cur_sq_dist;
                result = index_t(v);
            }
        }
        return result;
    }



    index_t Delaunay2d::locate_inexact(
        const double* p, index_t hint, index_t max_iter
    ) const {

        // If no hint specified, find a tetrahedron randomly
        while(hint == NO_TRIANGLE) {
            hint = index_t(Numeric::random_int32()) % max_t();
            if(triangle_is_free(hint)) {
                hint = NO_TRIANGLE;
            }
        }

	geo_debug_assert(!triangle_is_free(hint));
	geo_debug_assert(!triangle_is_in_list(hint));
	
        //  Always start from a real tet. If the tet is virtual,
        // find its real neighbor (always opposite to the
        // infinite vertex)
        if(triangle_is_virtual(hint)) {
            for(index_t lf = 0; lf < 3; ++lf) {
                if(triangle_vertex(hint, lf) == VERTEX_AT_INFINITY) {
                    hint = index_t(triangle_adjacent(hint, lf));
                    geo_debug_assert(hint != NO_TRIANGLE);
                    break;
                }
            }
        }

        index_t t = hint;
        index_t t_pred = NO_TRIANGLE;

    still_walking:
        {
            const double* pv[3];
            pv[0] = vertex_ptr(finite_triangle_vertex(t,0));
            pv[1] = vertex_ptr(finite_triangle_vertex(t,1));
            pv[2] = vertex_ptr(finite_triangle_vertex(t,2));
            
            for(index_t le = 0; le < 3; ++le) {
                
                signed_index_t s_t_next = triangle_adjacent(t,le);

                //  If the opposite tet is -1, then it means that
                // we are trying to locate() (e.g. called from
                // nearest_vertex) within a tetrahedralization 
                // from which the infinite tets were removed.
                if(s_t_next == -1) {
                    return NO_TRIANGLE;
                }

                index_t t_next = index_t(s_t_next);

                //   If the candidate next tetrahedron is the
                // one we came from, then we know already that
                // the orientation is positive, thus we examine
                // the next candidate (or exit the loop if they
                // are exhausted).
                if(t_next == t_pred) {
                    continue ; 
                }

                //   To test the orientation of p w.r.t. the facet f of
                // t, we replace vertex number f with p in t (same
                // convention as in CGAL).
                const double* pv_bkp = pv[le];
                pv[le] = p;
                Sign ori = orient_2d_inexact(pv[0], pv[1], pv[2]);

                //   If the orientation is not negative, then we cannot
                // walk towards t_next, and examine the next candidate
                // (or exit the loop if they are exhausted).
                if(ori != NEGATIVE) {
                    pv[le] = pv_bkp;
                    continue;
                }

                //  If the opposite tet is a virtual tet, then
                // the point has a positive orientation relative
                // to the facet on the border of the convex hull,
                // thus t_next is a tet in conflict and we are
                // done.
                if(triangle_is_virtual(t_next)) {
                    return t_next;
                }

                //   If we reach this point, then t_next is a valid
                // successor, thus we are still walking.
                t_pred = t;
                t = t_next;
                if(--max_iter != 0) {
                    goto still_walking;
                }
            }
        } 

        //   If we reach this point, we did not find a valid successor
        // for walking (a face for which p has negative orientation), 
        // thus we reached the tet for which p has all positive 
        // face orientations (i.e. the tet that contains p).

        return t;
    }


    index_t Delaunay2d::locate(
        const double* p, index_t hint, bool thread_safe,
        Sign* orient
    ) const {

        //   Try improving the hint by using the 
        // inexact locate function. This gains
        // (a little bit) performance (a few 
        // percent in total Delaunay computation
        // time), but it is better than nothing...
        //   Note: there is a maximum number of tets 
        // traversed by locate_inexact()  (2500)
        // since there exists configurations in which
        // locate_inexact() loops forever !

	hint = locate_inexact(p, hint, 2500);

        static Process::spinlock lock = GEOGRAM_SPINLOCK_INIT;

        // We need to have this spinlock because
        // of random() that is not thread-safe
        // (TODO: implement a random() function with
        //  thread local storage)
        if(thread_safe) {
            Process::acquire_spinlock(lock);
        }

        // If no hint specified, find a tetrahedron randomly
        while(hint == NO_TRIANGLE) {
            hint = index_t(Numeric::random_int32()) % max_t();
            if(triangle_is_free(hint)) {
                hint = NO_TRIANGLE;
            }
        }

	geo_debug_assert(!triangle_is_free(hint));
	geo_debug_assert(!triangle_is_in_list(hint));
	
        //  Always start from a real tet. If the tet is virtual,
        // find its real neighbor (always opposite to the
        // infinite vertex)
        if(triangle_is_virtual(hint)) {
            for(index_t le = 0; le < 3; ++le) {
                if(triangle_vertex(hint, le) == VERTEX_AT_INFINITY) {
                    hint = index_t(triangle_adjacent(hint, le));
                    geo_debug_assert(hint != NO_TRIANGLE);
                    break;
                }
            }
        }

	geo_debug_assert(!triangle_is_free(hint));
	geo_debug_assert(!triangle_is_in_list(hint));
	geo_debug_assert(!triangle_is_virtual(hint));	
	
        index_t t = hint;
        index_t t_pred = NO_TRIANGLE;
        Sign orient_local[3];
        if(orient == nullptr) {
            orient = orient_local;
        }


    still_walking:
        {
            const double* pv[3];
            pv[0] = vertex_ptr(finite_triangle_vertex(t,0));
            pv[1] = vertex_ptr(finite_triangle_vertex(t,1));
            pv[2] = vertex_ptr(finite_triangle_vertex(t,2));
            
            // Start from a random facet
            index_t e0 = index_t(Numeric::random_int32()) % 3;
            for(index_t de = 0; de < 3; ++de) {
                index_t le = (e0 + de) % 3;
                
                signed_index_t s_t_next = triangle_adjacent(t,le);

                //  If the opposite triangle is -1, then it means that
                // we are trying to locate() (e.g. called from
                // nearest_vertex) within a tetrahedralization 
                // from which the infinite tets were removed.
                if(s_t_next == -1) {
                    if(thread_safe) {
                        Process::release_spinlock(lock);
                    }
                    return NO_TRIANGLE;
                }

                index_t t_next = index_t(s_t_next);

		geo_debug_assert(!triangle_is_free(t_next));
		geo_debug_assert(!triangle_is_in_list(t_next));

		
                //   If the candidate next tetrahedron is the
                // one we came from, then we know already that
                // the orientation is positive, thus we examine
                // the next candidate (or exit the loop if they
                // are exhausted).
                if(t_next == t_pred) {
                    orient[le] = POSITIVE ;
                    continue ; 
                }

                //   To test the orientation of p w.r.t. the facet f of
                // t, we replace vertex number f with p in t (same
                // convention as in CGAL).
                // This is equivalent to tet_facet_point_orient3d(t,f,p)
                // (but less costly, saves a couple of lookups)
                const double* pv_bkp = pv[le];
                pv[le] = p;
                orient[le] = PCK::orient_2d(pv[0], pv[1], pv[2]);

                //   If the orientation is not negative, then we cannot
                // walk towards t_next, and examine the next candidate
                // (or exit the loop if they are exhausted).
                if(orient[le] != NEGATIVE) {
                    pv[le] = pv_bkp;
                    continue;
                }

                //  If the opposite tet is a virtual tet, then
                // the point has a positive orientation relative
                // to the facet on the border of the convex hull,
                // thus t_next is a tet in conflict and we are
                // done.
                if(triangle_is_virtual(t_next)) {
                    if(thread_safe) {
                        Process::release_spinlock(lock);
                    }
                    for(index_t tle = 0; tle < 3; ++tle) {
                        orient[tle] = POSITIVE;
                    }
                    return t_next;
                }

                //   If we reach this point, then t_next is a valid
                // successor, thus we are still walking.
                t_pred = t;
                t = t_next;
                goto still_walking;
            }
        } 

        //   If we reach this point, we did not find a valid successor
        // for walking (a face for which p has negative orientation), 
        // thus we reached the tet for which p has all positive 
        // face orientations (i.e. the tet that contains p).

        if(thread_safe) {
            Process::release_spinlock(lock);
        }
        return t;
    }

    void Delaunay2d::find_conflict_zone(
        index_t v, 
        index_t t, const Sign* orient, 
        index_t& t_bndry, index_t& e_bndry,
        index_t& first, index_t& last
    ) {
        first = last = END_OF_LIST;

        //  Generate a unique stamp from current vertex index,
        // used for marking tetrahedra.
        set_triangle_mark_stamp(v);

        // Pointer to the coordinates of the point to be inserted
        const double* p = vertex_ptr(v);

        geo_debug_assert(t != NO_TRIANGLE);

        // Test whether the point already exists in
        // the triangulation. The point already exists
        // if it's located on three faces of the
        // tetrahedron returned by locate().
        int nb_zero = 
            (orient[0] == ZERO) +
            (orient[1] == ZERO) +
            (orient[2] == ZERO) ;

        if(nb_zero >= 2) {
            return; 
        }

        //  Weighted triangulations can have dangling
        // vertices. Such vertices p are characterized by
        // the fact that p is not in conflict with the 
        // tetrahedron returned by locate().
        if(weighted_ && !triangle_is_conflict(t, p)) {
            return;
        }

        // Note: points on edges and on facets are
        // handled by the way triangle_is_in_conflict()
        // is implemented, that naturally inserts
        // the correct tetrahedra in the conflict list.


        // Mark t as conflict
        add_triangle_to_list(t, first, last);

        // A small optimization: if the point to be inserted
        // is on some faces of the located triangle, insert
        // the neighbors accros those edges in the conflict list.
        // It saves a couple of calls to the predicates in this
        // specific case (combinatorics are in general less 
        // expensive than the predicates).
        if(!weighted_ && nb_zero != 0) {
            for(index_t le = 0; le < 3; ++le) {
                if(orient[le] == ZERO) {
                    index_t t2 = index_t(triangle_adjacent(t, le));
                    add_triangle_to_list(t2, first, last);
                }
            }
            for(index_t le = 0; le < 3; ++le) {
                if(orient[le] == ZERO) {
                    index_t t2 = index_t(triangle_adjacent(t, le));
                    find_conflict_zone_iterative(
                        p,t2,t_bndry,e_bndry,first,last
                    );
                }
            }
        }

        // Determine the conflict list by greedy propagation from t.
        find_conflict_zone_iterative(p,t,t_bndry,e_bndry,first,last);
    }
    
    void Delaunay2d::find_conflict_zone_iterative(
        const double* p, index_t t_in,
        index_t& t_bndry, index_t& e_bndry,
        index_t& first, index_t& last
    ) {

        S_.push(t_in);

        while(!S_.empty()) {

            index_t t = S_.top();
            S_.pop();
            
            for(index_t le = 0; le < 3; ++le) {
                index_t t2 = index_t(triangle_adjacent(t, le));

                if(
                    triangle_is_in_list(t2) || // known as conflict
                    triangle_is_marked(t2)     // known as non-conflict
                ) {
                    continue;
                }

                if(triangle_is_conflict(t2, p)) {
                    // Chain t2 in conflict list
                    add_triangle_to_list(t2, first, last);
                    S_.push(t2);
                    continue;
                } 
                
                //   At this point, t is in conflict 
                // and t2 is not in conflict. 
                // We keep a reference to a tet on the boundary
                t_bndry = t;
                e_bndry = le;
                // Mark t2 as visited (but not conflict)
                mark_triangle(t2);
            }
        }
    }

    index_t Delaunay2d::stellate_conflict_zone(
        index_t v_in, index_t t1, index_t t1ebord
    ) {

	index_t t = t1;
	index_t e = t1ebord;
	index_t t_adj = index_t(triangle_adjacent(t,e));

	geo_debug_assert(t_adj != index_t(-1));
	
	geo_debug_assert(triangle_is_in_list(t));
	geo_debug_assert(!triangle_is_in_list(t_adj));
	

	index_t new_t_first = index_t(-1);
	index_t new_t_prev  = index_t(-1);
	
	do {

	    signed_index_t v1 = triangle_vertex(t, (e+1)%3);
	    signed_index_t v2 = triangle_vertex(t, (e+2)%3);	    

	    // Create new triangle
	    index_t new_t = new_triangle(signed_index_t(v_in), v1, v2);

	    //   Connect new triangle to triangle on the other
	    // side of the conflict zone.
	    set_triangle_adjacent(new_t, 0, t_adj);
	    index_t adj_e = find_triangle_adjacent(t_adj, t);
	    set_triangle_adjacent(t_adj, adj_e, new_t);
	    
	    
	    // Move to next triangle
	    e = (e + 1)%3;
	    t_adj = index_t(triangle_adjacent(t,e));
	    while(triangle_is_in_list(t_adj)) {
		t = t_adj;
		e = (find_triangle_vertex(t,v2) + 2)%3;		
		t_adj = index_t(triangle_adjacent(t,e));
		geo_debug_assert(t_adj != index_t(-1));		
	    }

	    if(new_t_prev == index_t(-1)) {
		new_t_first = new_t;
	    } else {
		set_triangle_adjacent(new_t_prev, 1, new_t);
		set_triangle_adjacent(new_t, 2, new_t_prev);
	    }

	    new_t_prev = new_t;
	    
	} while((t != t1) || (e != t1ebord));

	// Connect last triangle to first triangle
	set_triangle_adjacent(new_t_prev, 1, new_t_first);
	set_triangle_adjacent(new_t_first, 2, new_t_prev);
	
	return new_t_prev;
    }

    index_t Delaunay2d::insert(index_t v, index_t hint) {
       index_t t_bndry = NO_TRIANGLE;
       index_t e_bndry = index_t(-1);
       index_t first_conflict = NO_TRIANGLE;
       index_t last_conflict  = NO_TRIANGLE;

       const double* p = vertex_ptr(v);

       Sign orient[3];
       index_t t = locate(p, hint, false, orient);
       find_conflict_zone(
           v,t,orient,t_bndry,e_bndry,first_conflict,last_conflict
       );
       
       // The conflict list can be empty if:
       //  - Vertex v already exists in the triangulation
       //  - The triangulation is weighted and v is not visible
       if(first_conflict == END_OF_LIST) {
           return NO_TRIANGLE;
       }

       index_t new_triangle = stellate_conflict_zone(v,t_bndry,e_bndry);
       
       // Recycle the tetrahedra of the conflict zone.
       cell_next_[last_conflict] = first_free_;
       first_free_ = first_conflict;
       
       // Return one of the newly created triangles
       return new_triangle;
    }

    bool Delaunay2d::create_first_triangle(
        index_t& iv0, index_t& iv1, index_t& iv2
    ) {
        if(nb_vertices() < 3) {
            return false;
        }

        iv0 = 0;

        iv1 = 1;
        while(
            iv1 < nb_vertices() &&
            PCK::points_are_identical_2d(
                vertex_ptr(iv0), vertex_ptr(iv1)
            )
        ) {
            ++iv1;
        }
        if(iv1 == nb_vertices()) {
            return false;
        }

        iv2 = iv1 + 1;
	Sign s = ZERO;
        while(
            iv2 < nb_vertices() &&  
	    (s = PCK::orient_2d(vertex_ptr(iv0), vertex_ptr(iv1), vertex_ptr(iv2))) == ZERO
	) {
            ++iv2;
        }
        if(iv2 == nb_vertices()) {
            return false;
        }
	if(s == NEGATIVE) {
	    std::swap(iv1,iv2);
	}
	    
        // Create the first triangle
        index_t t0 = new_triangle(
            signed_index_t(iv0), 
            signed_index_t(iv1), 
            signed_index_t(iv2)
        );

        // Create the first three virtual triangles surrounding it
        index_t t[3];
        for(index_t e = 0; e < 3; ++e) {
            // In reverse order since it is an adjacent tetrahedron
            signed_index_t v1 = triangle_vertex(t0, triangle_edge_vertex(e,1));
            signed_index_t v2 = triangle_vertex(t0, triangle_edge_vertex(e,0));
            t[e] = new_triangle(VERTEX_AT_INFINITY, v1, v2);
        }

        // Connect the virtual triangles to the real one
        for(index_t e=0; e<3; ++e) {
            set_triangle_adjacent(t[e], 0, t0);
            set_triangle_adjacent(t0, e, t[e]);
        }

        // Interconnect the three virtual triangles along their common
        // edges
        for(index_t e = 0; e < 3; ++e) {
            // In reverse order since it is an adjacent tetrahedron
            index_t lv1 = triangle_edge_vertex(e,1);
            index_t lv2 = triangle_edge_vertex(e,0);
            set_triangle_adjacent(t[e], 1, t[lv1]);
            set_triangle_adjacent(t[e], 2, t[lv2]);
        }

        return true;
    }

    

    void Delaunay2d::show_triangle(index_t t) const {
        std::cerr << "tri"
            << (triangle_is_in_list(t) ? '*' : ' ')
            << t
            << ", v=["
            << triangle_vertex(t, 0)
            << ' '
            << triangle_vertex(t, 1)
            << ' '
            << triangle_vertex(t, 2)
            << "]  adj=[";
        show_triangle_adjacent(t, 0);
        show_triangle_adjacent(t, 1);
        show_triangle_adjacent(t, 2);
        std::cerr << "] ";

        for(index_t e = 0; e < 3; ++e) {
            std::cerr << 'e' << e << ':';
            for(index_t v = 0; v < 2; ++v) {
                std::cerr << triangle_vertex(t, triangle_edge_vertex(e,v))
			  << ',';
            }
            std::cerr << ' ';
        }
        std::cerr << std::endl;
    }

    void Delaunay2d::show_triangle_adjacent(index_t t, index_t le) const {
        signed_index_t adj = triangle_adjacent(t, le);
        if(adj != -1) {
            std::cerr << (triangle_is_in_list(index_t(adj)) ? '*' : ' ');
        }
        std::cerr << adj;
        std::cerr << ' ';
    }

    void Delaunay2d::show_list(
        index_t first, const std::string& list_name
    ) const {
        index_t t = first;
        std::cerr << "tri list: " << list_name << std::endl;
        while(t != END_OF_LIST) {
            show_triangle(t);
            t = triangle_next(t);
        }
        std::cerr << "-------------" << std::endl;
    }

    void Delaunay2d::check_combinatorics(bool verbose) const {
        if(verbose) {
            std::cerr << std::endl;
        }
        bool ok = true;
        std::vector<bool> v_has_triangle(nb_vertices(), false);
        for(index_t t = 0; t < max_t(); ++t) {
            if(triangle_is_free(t)) {
/*
                if(verbose) {
                    std::cerr << "-Deleted tri: ";
                    show_tri(t);
                }
*/
            } else {
/*
                if(verbose) {
                    std::cerr << "Checking tri: ";
                    show_tet(t);
                }
*/
                for(index_t le = 0; le < 3; ++le) {
                    if(triangle_adjacent(t, le) == -1) {
                        std::cerr << le << ":Missing adjacent tri"
                            << std::endl;
                        ok = false;
                    } else if(triangle_adjacent(t, le) == signed_index_t(t)) {
                        std::cerr << le << ":Tri is adjacent to itself"
                            << std::endl;
                        ok = false;
                    } else {
                        index_t t2 = index_t(triangle_adjacent(t, le));
                        bool found = false;
                        for(index_t le2 = 0; le2 < 3; ++le2) {
                            if(triangle_adjacent(t2, le2) == signed_index_t(t)) {
                                found = true;
                            }
                        }
                        if(!found) {
                            std::cerr
                                << le << ":Adjacent link is not bidirectional"
                                << std::endl;
                            ok = false;
                        }
                    }
                }
                index_t nb_infinite = 0;
                for(index_t lv = 0; lv < 3; ++lv) {
                    if(triangle_vertex(t, lv) == -1) {
                        ++nb_infinite;
                    }
                }
                if(nb_infinite > 1) {
                    ok = false;
                    std::cerr << "More than one infinite vertex"
                        << std::endl;
                }
            }
            for(index_t lv = 0; lv < 3; ++lv) {
                signed_index_t v = triangle_vertex(t, lv);
                if(v >= 0) {
                    v_has_triangle[index_t(v)] = true;
                }
            }
        }
        for(index_t v = 0; v < nb_vertices(); ++v) {
            if(!v_has_triangle[v]) {
                if(verbose) {
                    std::cerr << "Vertex " << v
                        << " is isolated (duplicated ?)" << std::endl;
                }
            }
        }
        geo_assert(ok);
        if(verbose) {
            std::cerr << std::endl;
        }
        std::cerr << std::endl << "Delaunay Combi OK" << std::endl;
    }

    void Delaunay2d::check_geometry(bool verbose) const {
        bool ok = true;
        for(index_t t = 0; t < max_t(); ++t) {
            if(!triangle_is_free(t)) {
                signed_index_t v0 = triangle_vertex(t, 0);
                signed_index_t v1 = triangle_vertex(t, 1);
                signed_index_t v2 = triangle_vertex(t, 2);
                for(index_t v = 0; v < nb_vertices(); ++v) {
                    signed_index_t sv = signed_index_t(v);
                    if(sv == v0 || sv == v1 || sv == v2) {
                        continue;
                    }
                    if(triangle_is_conflict(t, vertex_ptr(v))) {
                        ok = false;
                        if(verbose) {
                            std::cerr << "Tri " << t <<
                                " is in conflict with vertex " << v
                                    << std::endl;

                            std::cerr << "  offending tri: ";
                            show_triangle(t);
                        }
                    }
                }
            }
        }
        geo_assert(ok);
        std::cerr << std::endl << "Delaunay Geo OK" << std::endl;
    }

    

    RegularWeightedDelaunay2d::RegularWeightedDelaunay2d(
        coord_index_t dimension
    ) :
        Delaunay2d(3)
    {
        if(dimension != 3) {
            throw InvalidDimension(dimension, "RegularWeightedDelaunay2d", "3");
        }
    }

    RegularWeightedDelaunay2d::~RegularWeightedDelaunay2d() {
    }
}


/******* extracted from delaunay_3d.cpp *******/

#include <stack>

// TODO: optimizations:
// - convex hull traversal for nearest_vertex()

namespace GEO {

    char Delaunay3d::halfedge_facet_[4][4] = {
        {4, 2, 3, 1},
        {3, 4, 0, 2},
        {1, 3, 4, 0},
        {2, 0, 1, 4}
    };

    // tet facet vertex is such that the tetrahedron
    // formed with:
    //  vertex lv
    //  tet_facet_vertex[lv][0]
    //  tet_facet_vertex[lv][1]
    //  tet_facet_vertex[lv][2]
    // has the same orientation as the original tetrahedron for
    // any vertex lv.

    char Delaunay3d::tet_facet_vertex_[4][3] = {
        {1, 2, 3},
        {0, 3, 2},
        {3, 0, 1},
        {1, 0, 2}
    };

    Delaunay3d::Delaunay3d(coord_index_t dimension) :
        Delaunay(dimension)
    {
	geo_cite_with_info(
	    "DBLP:journals/cj/Bowyer81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
        );
	geo_cite_with_info(
	    "journals/cj/Watson81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
	);
	geo_cite_with_info(
	    "DBLP:conf/compgeom/AmentaCR03",
	    "Using spatial sorting has a dramatic impact on the performances."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/FunkeMN05",
	    "Initializing \\verb|locate()| with a non-exact version "
	    " (structural filtering) gains (a bit of) performance."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/BoissonnatDPTY02",
	    "The idea of traversing the cavity from inside "
	    " used in GEOGRAM is inspired by the implementation of "
	    " \\verb|Delaunay_triangulation_3| in CGAL."
	);
	geo_cite_with_info(
	    "DBLP:conf/imr/Si06",
	    "The triangulation data structure used in GEOGRAM is inspired "
	    "by Tetgen."
	);
	geo_cite_with_info(
	    "DBLP:journals/ijfcs/DevillersPT02",
	    "Analysis of the different versions of the line walk algorithm "
	    " used by \\verb|locate()|."
	);
	
        if(dimension != 3 && dimension != 4) {
            throw InvalidDimension(dimension, "Delaunay3d", "3 or 4");
        }
        first_free_ = END_OF_LIST;
        weighted_ = (dimension == 4);
        // In weighted mode, vertices are 4d but combinatorics is 3d.
        if(weighted_) {
            cell_size_ = 4;
            cell_v_stride_ = 4;
            cell_neigh_stride_ = 4;
        }
        cur_stamp_ = 0;
        debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay");
        verbose_debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay_verbose");
        debug_mode_ = (debug_mode_ || verbose_debug_mode_);
        benchmark_mode_ = CmdLine::get_arg_bool("dbg:delaunay_benchmark");
    }

    Delaunay3d::~Delaunay3d() {
    }

    void Delaunay3d::set_vertices(
        index_t nb_vertices, const double* vertices
    ) {
        Stopwatch* W = nullptr;
        if(benchmark_mode_) {
            W = new Stopwatch("DelInternal");
        }
        cur_stamp_ = 0;
        if(weighted_) {
            heights_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                // Client code uses 4d embedding with ti = sqrt(W - wi)
                //   where W = max(wi)
                // We recompute the standard "shifted" lifting on
                // the paraboloid from it.
                // (we use wi - W, everything is shifted by W, but
                // we do not care since the power diagram is invariant
                // by a translation of all weights).
                double w = -geo_sqr(vertices[4 * i + 3]);
                heights_[i] = -w +
                    geo_sqr(vertices[4 * i]) +
                    geo_sqr(vertices[4 * i + 1]) +
                    geo_sqr(vertices[4 * i + 2]);
            }
        }

        Delaunay::set_vertices(nb_vertices, vertices);

        index_t expected_tetra = nb_vertices * 7;

        cell_to_v_store_.reserve(expected_tetra * 4);
        cell_to_cell_store_.reserve(expected_tetra * 4);
        cell_next_.reserve(expected_tetra);

        cell_to_v_store_.resize(0);
        cell_to_cell_store_.resize(0);
        cell_next_.resize(0);
        first_free_ = END_OF_LIST;

        //   Sort the vertices spatially. This makes localisation
        // faster.
        if(do_reorder_) {
            compute_BRIO_order(
                nb_vertices, vertex_ptr(0), reorder_, 3, dimension_
            );
        } else {
            reorder_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                reorder_[i] = i;
            }
        }

        double sorting_time = 0;
        if(benchmark_mode_) {
            sorting_time = W->elapsed_time();
            Logger::out("DelInternal1") << "BRIO sorting:"
                                       << sorting_time
                                       << std::endl;
        } 

        // The indices of the vertices of the first tetrahedron.
        index_t v0, v1, v2, v3;
        if(!create_first_tetrahedron(v0, v1, v2, v3)) {
            Logger::warn("Delaunay3d") << "All the points are coplanar"
                << std::endl;
            return;
        }

        index_t hint = NO_TETRAHEDRON;
        // Insert all the vertices incrementally.
        for(index_t i = 0; i < nb_vertices; ++i) {
            index_t v = reorder_[i];
            // Do not re-insert the first four vertices.
            if(v != v0 && v != v1 && v != v2 && v != v3) {
                index_t new_hint = insert(v, hint);
                if(new_hint != NO_TETRAHEDRON) {
                    hint = new_hint;
                }
            }
        }

        if(benchmark_mode_) {
            Logger::out("DelInternal2") << "Core insertion algo:"
                                       << W->elapsed_time() - sorting_time
                                       << std::endl;
        }
        delete W;

        if(debug_mode_) {
            check_combinatorics(verbose_debug_mode_);
            check_geometry(verbose_debug_mode_);
        }

        //   Compress cell_to_v_store_ and cell_to_cell_store_
        // (remove free and virtual tetrahedra).
        //   Since cell_next_ is not used at this point,
        // we reuse it for storing the conversion array that
        // maps old tet indices to new tet indices
        // Note: tet_is_real() uses the previous value of 
        // cell_next(), but we are processing indices
        // in increasing order and since old2new[t] is always
        // smaller or equal to t, we never overwrite a value
        // before needing it.
        
        vector<index_t>& old2new = cell_next_;
        index_t nb_tets = 0;
        index_t nb_tets_to_delete = 0;
        
        {
            for(index_t t = 0; t < max_t(); ++t) {
                if(
                    (keep_infinite_ && !tet_is_free(t)) ||
                    tet_is_real(t)
                ) {
                    if(t != nb_tets) {
                        Memory::copy(
                            &cell_to_v_store_[nb_tets * 4],
                            &cell_to_v_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                        Memory::copy(
                            &cell_to_cell_store_[nb_tets * 4],
                            &cell_to_cell_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                    }
                    old2new[t] = nb_tets;
                    ++nb_tets;
                } else {
                    old2new[t] = index_t(-1);
                    ++nb_tets_to_delete;
                }
            }
            cell_to_v_store_.resize(4 * nb_tets);
            cell_to_cell_store_.resize(4 * nb_tets);
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                // Note: t can be equal to -1 when a real tet is
                // adjacent to a virtual one (and this is how the
                // rest of Vorpaline expects to see tets on the
                // border).
                cell_to_cell_store_[i] = t;
            }
        }

        // In "keep_infinite" mode, we reorder the cells in such
        // a way that finite cells have indices [0..nb_finite_cells_-1]
        // and infinite cells have indices [nb_finite_cells_ .. nb_cells_-1]
        
        if(keep_infinite_) {
            nb_finite_cells_ = 0;
            index_t finite_ptr = 0;
            index_t infinite_ptr = nb_tets - 1;
            for(;;) {
                while(tet_is_finite(finite_ptr)) {
                    old2new[finite_ptr] = finite_ptr;
                    ++finite_ptr;
                    ++nb_finite_cells_;
                }
                while(!tet_is_finite(infinite_ptr)) {
                    old2new[infinite_ptr] = infinite_ptr;
                    --infinite_ptr;
                }
                if(finite_ptr > infinite_ptr) {
                    break;
                }
                old2new[finite_ptr] = infinite_ptr;
                old2new[infinite_ptr] = finite_ptr;
                ++nb_finite_cells_;
                for(index_t lf=0; lf<4; ++lf) {
                    std::swap(
                        cell_to_cell_store_[4*finite_ptr + lf],
                        cell_to_cell_store_[4*infinite_ptr + lf]
                    );
                }
                for(index_t lv=0; lv<4; ++lv) {
                    std::swap(
                        cell_to_v_store_[4*finite_ptr + lv],
                        cell_to_v_store_[4*infinite_ptr + lv]
                    );
                }
                ++finite_ptr;
                --infinite_ptr;
            }
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                geo_debug_assert(t >= 0);
                cell_to_cell_store_[i] = t;
            }
        }

        if(benchmark_mode_) {
            if(keep_infinite_) {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list)" << std::endl;
            } else {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list and infinite)" << std::endl;
            }
        }
        
        set_arrays(
            nb_tets,
            cell_to_v_store_.data(), cell_to_cell_store_.data()
        );
    }

    index_t Delaunay3d::nearest_vertex(const double* p) const {

        // TODO: For the moment, we fallback to the (unefficient)
        // baseclass implementation when in weighted mode.
        if(weighted_) {
            return Delaunay::nearest_vertex(p);
        }

        // Find a tetrahedron (real or virtual) that contains p
        index_t t = locate(p, NO_TETRAHEDRON, thread_safe());

        //   If p is outside the convex hull of the inserted points,
        // a special traversal is required (not implemented yet).
        // TODO: implement convex hull boundary traversal
        // (for now we fallback to linear search implemented
        //  in baseclass)
        if(t == NO_TETRAHEDRON || tet_is_virtual(t)) {
            return Delaunay::nearest_vertex(p);
        }

        double sq_dist = 1e30;
        index_t result = NO_TETRAHEDRON;

        // Find the nearest vertex among t's vertices
        for(index_t lv = 0; lv < 4; ++lv) {
            signed_index_t v = tet_vertex(t, lv);
            // If the tetrahedron is virtual, then the first vertex
            // is the vertex at infinity and is skipped.
            if(v < 0) {
                continue;
            }
            double cur_sq_dist = Geom::distance2(p, vertex_ptr(index_t(v)), 3);
            if(cur_sq_dist < sq_dist) {
                sq_dist = cur_sq_dist;
                result = index_t(v);
            }
        }
        return result;
    }



    index_t Delaunay3d::locate_inexact(
        const double* p, index_t hint, index_t max_iter
    ) const {

        // If no hint specified, find a tetrahedron randomly
        while(hint == NO_TETRAHEDRON) {
            hint = index_t(Numeric::random_int32()) % max_t();
            if(tet_is_free(hint)) {
                hint = NO_TETRAHEDRON;
            }
        }

        //  Always start from a real tet. If the tet is virtual,
        // find its real neighbor (always opposite to the
        // infinite vertex)
        if(tet_is_virtual(hint)) {
            for(index_t lf = 0; lf < 4; ++lf) {
                if(tet_vertex(hint, lf) == VERTEX_AT_INFINITY) {
                    hint = index_t(tet_adjacent(hint, lf));
                    geo_debug_assert(hint != NO_TETRAHEDRON);
                    break;
                }
            }
        }

        index_t t = hint;
        index_t t_pred = NO_TETRAHEDRON;

    still_walking:
        {
            const double* pv[4];
            pv[0] = vertex_ptr(finite_tet_vertex(t,0));
            pv[1] = vertex_ptr(finite_tet_vertex(t,1));
            pv[2] = vertex_ptr(finite_tet_vertex(t,2));
            pv[3] = vertex_ptr(finite_tet_vertex(t,3));
            
            for(index_t f = 0; f < 4; ++f) {
                
                signed_index_t s_t_next = tet_adjacent(t,f);

                //  If the opposite tet is -1, then it means that
                // we are trying to locate() (e.g. called from
                // nearest_vertex) within a tetrahedralization 
                // from which the infinite tets were removed.
                if(s_t_next == -1) {
                    return NO_TETRAHEDRON;
                }

                index_t t_next = index_t(s_t_next);

                //   If the candidate next tetrahedron is the
                // one we came from, then we know already that
                // the orientation is positive, thus we examine
                // the next candidate (or exit the loop if they
                // are exhausted).
                if(t_next == t_pred) {
                    continue ; 
                }

                //   To test the orientation of p w.r.t. the facet f of
                // t, we replace vertex number f with p in t (same
                // convention as in CGAL).
                const double* pv_bkp = pv[f];
                pv[f] = p;
                Sign ori = PCK::orient_3d_inexact(pv[0], pv[1], pv[2], pv[3]);

                //   If the orientation is not negative, then we cannot
                // walk towards t_next, and examine the next candidate
                // (or exit the loop if they are exhausted).
                if(ori != NEGATIVE) {
                    pv[f] = pv_bkp;
                    continue;
                }

                //  If the opposite tet is a virtual tet, then
                // the point has a positive orientation relative
                // to the facet on the border of the convex hull,
                // thus t_next is a tet in conflict and we are
                // done.
                if(tet_is_virtual(t_next)) {
                    return t_next;
                }

                //   If we reach this point, then t_next is a valid
                // successor, thus we are still walking.
                t_pred = t;
                t = t_next;
                if(--max_iter != 0) {
                    goto still_walking;
                }
            }
        } 

        //   If we reach this point, we did not find a valid successor
        // for walking (a face for which p has negative orientation), 
        // thus we reached the tet for which p has all positive 
        // face orientations (i.e. the tet that contains p).

        return t;
    }


    index_t Delaunay3d::locate(
        const double* p, index_t hint, bool thread_safe,
        Sign* orient
    ) const {

        //   Try improving the hint by using the 
        // inexact locate function. This gains
        // (a little bit) performance (a few 
        // percent in total Delaunay computation
        // time), but it is better than nothing...
        //   Note: there is a maximum number of tets 
        // traversed by locate_inexact()  (2500)
        // since there exists configurations in which
        // locate_inexact() loops forever !
        hint = locate_inexact(p, hint, 2500);

        static Process::spinlock lock = GEOGRAM_SPINLOCK_INIT;

        // We need to have this spinlock because
        // of random() that is not thread-safe
        // (TODO: implement a random() function with
        //  thread local storage)
        if(thread_safe) {
            Process::acquire_spinlock(lock);
        }

        // If no hint specified, find a tetrahedron randomly
        while(hint == NO_TETRAHEDRON) {
            hint = index_t(Numeric::random_int32()) % max_t();
            if(tet_is_free(hint)) {
                hint = NO_TETRAHEDRON;
            }
        }

        //  Always start from a real tet. If the tet is virtual,
        // find its real neighbor (always opposite to the
        // infinite vertex)
        if(tet_is_virtual(hint)) {
            for(index_t lf = 0; lf < 4; ++lf) {
                if(tet_vertex(hint, lf) == VERTEX_AT_INFINITY) {
                    hint = index_t(tet_adjacent(hint, lf));
                    geo_debug_assert(hint != NO_TETRAHEDRON);
                    break;
                }
            }
        }

        index_t t = hint;
        index_t t_pred = NO_TETRAHEDRON;
        Sign orient_local[4];
        if(orient == nullptr) {
            orient = orient_local;
        }


    still_walking:
        {
            const double* pv[4];
            pv[0] = vertex_ptr(finite_tet_vertex(t,0));
            pv[1] = vertex_ptr(finite_tet_vertex(t,1));
            pv[2] = vertex_ptr(finite_tet_vertex(t,2));
            pv[3] = vertex_ptr(finite_tet_vertex(t,3));
            
            // Start from a random facet
            index_t f0 = index_t(Numeric::random_int32()) % 4;
            for(index_t df = 0; df < 4; ++df) {
                index_t f = (f0 + df) % 4;
                
                signed_index_t s_t_next = tet_adjacent(t,f);

                //  If the opposite tet is -1, then it means that
                // we are trying to locate() (e.g. called from
                // nearest_vertex) within a tetrahedralization 
                // from which the infinite tets were removed.
                if(s_t_next == -1) {
                    if(thread_safe) {
                        Process::release_spinlock(lock);
                    }
                    return NO_TETRAHEDRON;
                }

                index_t t_next = index_t(s_t_next);

                //   If the candidate next tetrahedron is the
                // one we came from, then we know already that
                // the orientation is positive, thus we examine
                // the next candidate (or exit the loop if they
                // are exhausted).
                if(t_next == t_pred) {
                    orient[f] = POSITIVE ;
                    continue ; 
                }

                //   To test the orientation of p w.r.t. the facet f of
                // t, we replace vertex number f with p in t (same
                // convention as in CGAL).
                // This is equivalent to tet_facet_point_orient3d(t,f,p)
                // (but less costly, saves a couple of lookups)
                const double* pv_bkp = pv[f];
                pv[f] = p;
                orient[f] = PCK::orient_3d(pv[0], pv[1], pv[2], pv[3]);

                //   If the orientation is not negative, then we cannot
                // walk towards t_next, and examine the next candidate
                // (or exit the loop if they are exhausted).
                if(orient[f] != NEGATIVE) {
                    pv[f] = pv_bkp;
                    continue;
                }

                //  If the opposite tet is a virtual tet, then
                // the point has a positive orientation relative
                // to the facet on the border of the convex hull,
                // thus t_next is a tet in conflict and we are
                // done.
                if(tet_is_virtual(t_next)) {
                    if(thread_safe) {
                        Process::release_spinlock(lock);
                    }
                    for(index_t lf = 0; lf < 4; ++lf) {
                        orient[lf] = POSITIVE;
                    }
                    return t_next;
                }

                //   If we reach this point, then t_next is a valid
                // successor, thus we are still walking.
                t_pred = t;
                t = t_next;
                goto still_walking;
            }
        } 

        //   If we reach this point, we did not find a valid successor
        // for walking (a face for which p has negative orientation), 
        // thus we reached the tet for which p has all positive 
        // face orientations (i.e. the tet that contains p).

        if(thread_safe) {
            Process::release_spinlock(lock);
        }
        return t;
    }

    void Delaunay3d::find_conflict_zone(
        index_t v, 
        index_t t, const Sign* orient, 
        index_t& t_bndry, index_t& f_bndry,
        index_t& first, index_t& last
    ) {
	cavity_.clear(); 
	
        first = last = END_OF_LIST;

        //  Generate a unique stamp from current vertex index,
        // used for marking tetrahedra.
        set_tet_mark_stamp(v);

        // Pointer to the coordinates of the point to be inserted
        const double* p = vertex_ptr(v);

        geo_debug_assert(t != NO_TETRAHEDRON);

        // Test whether the point already exists in
        // the triangulation. The point already exists
        // if it's located on three faces of the
        // tetrahedron returned by locate().
        int nb_zero = 
            (orient[0] == ZERO) +
            (orient[1] == ZERO) +
            (orient[2] == ZERO) +
            (orient[3] == ZERO) ;

        if(nb_zero >= 3) {
            return; 
        }

        //  Weighted triangulations can have dangling
        // vertices. Such vertices p are characterized by
        // the fact that p is not in conflict with the 
        // tetrahedron returned by locate().
        if(weighted_ && !tet_is_conflict(t, p)) {
            return;
        }

        // Note: points on edges and on facets are
        // handled by the way tet_is_in_conflict()
        // is implemented, that naturally inserts
        // the correct tetrahedra in the conflict list.


        // Mark t as conflict
        add_tet_to_list(t, first, last);

        // A small optimization: if the point to be inserted
        // is on some faces of the located tetrahedron, insert
        // the neighbors accros those faces in the conflict list.
        // It saves a couple of calls to the predicates in this
        // specific case (combinatorics are in general less 
        // expensive than the predicates).
        if(!weighted_ && nb_zero != 0) {
            for(index_t lf = 0; lf < 4; ++lf) {
                if(orient[lf] == ZERO) {
                    index_t t2 = index_t(tet_adjacent(t, lf));
                    add_tet_to_list(t2, first, last);
                }
            }
            for(index_t lf = 0; lf < 4; ++lf) {
                if(orient[lf] == ZERO) {
                    index_t t2 = index_t(tet_adjacent(t, lf));
                    find_conflict_zone_iterative(
                        p,t2,t_bndry,f_bndry,first,last
                    );
                }
            }
        }

        // Determine the conflict list by greedy propagation from t.
        find_conflict_zone_iterative(p,t,t_bndry,f_bndry,first,last);
    }
    
    void Delaunay3d::find_conflict_zone_iterative(
        const double* p, index_t t_in,
        index_t& t_bndry, index_t& f_bndry,
        index_t& first, index_t& last
    ) {

        //std::stack<index_t> S;
        S_.push(t_in);

        while(!S_.empty()) {

            index_t t = S_.top();
            S_.pop();
            
            for(index_t lf = 0; lf < 4; ++lf) {
                index_t t2 = index_t(tet_adjacent(t, lf));

                if(
                    tet_is_in_list(t2)  // known as conflict
                ) {
                    continue;
                }

                if(
                    tet_is_marked(t2)     // known as non-conflict
                ) {
		    cavity_.new_facet(
			t, lf,
			tet_vertex(t, tet_facet_vertex(lf,0)),
			tet_vertex(t, tet_facet_vertex(lf,1)),
			tet_vertex(t, tet_facet_vertex(lf,2))
		    );
                    continue;
                }
		

                if(tet_is_conflict(t2, p)) {
                    // Chain t2 in conflict list
                    add_tet_to_list(t2, first, last);
                    S_.push(t2);
                    continue;
                } 
                
                //   At this point, t is in conflict 
                // and t2 is not in conflict. 
                // We keep a reference to a tet on the boundary
                t_bndry = t;
                f_bndry = lf;
                // Mark t2 as visited (but not conflict)
                mark_tet(t2);

		cavity_.new_facet(
		    t, lf,
		    tet_vertex(t, tet_facet_vertex(lf,0)),
		    tet_vertex(t, tet_facet_vertex(lf,1)),
		    tet_vertex(t, tet_facet_vertex(lf,2))
		);
		
            }
        }
    }

    index_t Delaunay3d::stellate_conflict_zone_iterative(
        index_t v_in, index_t t1, index_t t1fbord, index_t t1fprev
    ) {
        //   This function is de-recursified because some degenerate
        // inputs can cause stack overflow (system stack is limited to
        // a few megs). For instance, it can happen when a large number
        // of points are on the same sphere exactly.
        
        //   To de-recursify, it uses class StellateConflictStack
        // that emulates system's stack for storing functions's
        // parameters and local variables in all the nested stack
        // frames. 
        
        signed_index_t v = signed_index_t(v_in);
        
        S2_.push(t1, t1fbord, t1fprev);

        index_t new_t;   // the newly created tetrahedron.
        
        index_t t1ft2;   // traverses the 4 facets of t1.
        
        index_t t2;      // the tetrahedron on the border of
                         // the conflict zone that shares an
                         // edge with t1 along t1ft2.
        
        index_t t2fbord; // the facet of t2 on the border of
                         // the conflict zone.
        
        index_t t2ft1;   // the facet of t2 that is incident to t1.
        
    entry_point:
        S2_.get_parameters(t1, t1fbord, t1fprev);
        
        geo_debug_assert(tet_is_in_list(t1));
        geo_debug_assert(tet_adjacent(t1,t1fbord)>=0);
        geo_debug_assert(!tet_is_in_list(index_t(tet_adjacent(t1,t1fbord))));

        // Create new tetrahedron with same vertices as t_bndry
        new_t = new_tetrahedron(
            tet_vertex(t1,0),
            tet_vertex(t1,1),
            tet_vertex(t1,2),
            tet_vertex(t1,3)
        );

        // Replace in new_t the vertex opposite to t1fbord with v
        set_tet_vertex(new_t, t1fbord, v);

        // Connect new_t with t1's neighbor accros t1fbord
        {
            index_t tbord = index_t(tet_adjacent(t1,t1fbord));
            set_tet_adjacent(new_t, t1fbord, tbord);
            set_tet_adjacent(tbord, find_tet_adjacent(tbord,t1), new_t);
        }
            
        //  Lookup new_t's neighbors accros its three other
        // facets and connect them
        for(t1ft2=0; t1ft2<4; ++t1ft2) {
            
            if(t1ft2 == t1fprev || tet_adjacent(new_t,t1ft2) != -1) {
                continue;
            }

            // Get t1's neighbor along the border of the conflict zone
            if(!get_neighbor_along_conflict_zone_border(
                   t1,t1fbord,t1ft2, t2,t2fbord,t2ft1
            )) {
                //   If t1's neighbor is not a new tetrahedron,
                // create a new tetrahedron through a recursive call.
                S2_.save_locals(new_t, t1ft2, t2ft1);
                S2_.push(t2, t2fbord, t2ft1);
                goto entry_point;

            return_point:
                // This is the return value of the called function.
                index_t result = new_t;
                S2_.pop();

                // Special case: we were in the outermost frame, 
                // then we (truly) return from the function.
                if(S2_.empty()) {
                    return result;
                }
                
                S2_.get_parameters(t1, t1fbord, t1fprev);
                S2_.get_locals(new_t, t1ft2, t2ft1); 
                t2 = result; 
            }

            set_tet_adjacent(t2, t2ft1, new_t);
            set_tet_adjacent(new_t, t1ft2, t2);
        }

        // Except for the initial call (see "Special case" above),
        // the nested calls all come from the same location,
        // thus there is only one possible return point
        // (no need to push any return address).
        goto return_point;
    }
                        
    index_t Delaunay3d::stellate_cavity(index_t v) {
	
	index_t new_tet = index_t(-1);
	
	for(index_t f=0; f<cavity_.nb_facets(); ++f) {
	    index_t old_tet = cavity_.facet_tet(f);
	    index_t lf = cavity_.facet_facet(f);
	    index_t t_neigh = index_t(tet_adjacent(old_tet, lf));
	    signed_index_t v1 = cavity_.facet_vertex(f,0);
	    signed_index_t v2 = cavity_.facet_vertex(f,1);
	    signed_index_t v3 = cavity_.facet_vertex(f,2);
	    new_tet = new_tetrahedron(signed_index_t(v), v1, v2, v3);
	    set_tet_adjacent(new_tet, 0, t_neigh);
	    set_tet_adjacent(t_neigh, find_tet_adjacent(t_neigh,old_tet), new_tet);
	    cavity_.set_facet_tet(f, new_tet);
	}
	
	for(index_t f=0; f<cavity_.nb_facets(); ++f) {
	    new_tet = cavity_.facet_tet(f);
	    index_t neigh1, neigh2, neigh3;
	    cavity_.get_facet_neighbor_tets(f, neigh1, neigh2, neigh3);
	    set_tet_adjacent(new_tet, 1, neigh1);
	    set_tet_adjacent(new_tet, 2, neigh2);
	    set_tet_adjacent(new_tet, 3, neigh3);		
	}

	return new_tet;
    }
    
    index_t Delaunay3d::insert(index_t v, index_t hint) {
       index_t t_bndry = NO_TETRAHEDRON;
       index_t f_bndry = index_t(-1);
       index_t first_conflict = NO_TETRAHEDRON;
       index_t last_conflict = NO_TETRAHEDRON;

       const double* p = vertex_ptr(v);

       Sign orient[4];
       index_t t = locate(p, hint, false, orient);
       find_conflict_zone(
           v,t,orient,t_bndry,f_bndry,first_conflict,last_conflict
       );
       
       // The conflict list can be empty if:
       //  - Vertex v already exists in the triangulation
       //  - The triangulation is weighted and v is not visible
       if(first_conflict == END_OF_LIST) {
           return NO_TETRAHEDRON;
       }

       index_t new_tet = index_t(-1);
       if(cavity_.OK()) {
	   new_tet = stellate_cavity(v);
       } else {
	   new_tet = stellate_conflict_zone_iterative(v,t_bndry,f_bndry);
       }
       
       // Recycle the tetrahedra of the conflict zone.
       cell_next_[last_conflict] = first_free_;
       first_free_ = first_conflict;
       
       // Return one of the newly created tets
       return new_tet;
    }

    bool Delaunay3d::create_first_tetrahedron(
        index_t& iv0, index_t& iv1, index_t& iv2, index_t& iv3
    ) {
        if(nb_vertices() < 4) {
            return false;
        }

        iv0 = 0;

        iv1 = 1;
        while(
            iv1 < nb_vertices() &&
            PCK::points_are_identical_3d(
                vertex_ptr(iv0), vertex_ptr(iv1)
            )
        ) {
            ++iv1;
        }
        if(iv1 == nb_vertices()) {
            return false;
        }

        iv2 = iv1 + 1;
        while(
            iv2 < nb_vertices() &&
            PCK::points_are_colinear_3d(
                vertex_ptr(iv0), vertex_ptr(iv1), vertex_ptr(iv2)
            )
        ) {
            ++iv2;
        }
        if(iv2 == nb_vertices()) {
            return false;
        }

        iv3 = iv2 + 1;
        Sign s = ZERO;
        while(
            iv3 < nb_vertices() &&
            (s = PCK::orient_3d(
                    vertex_ptr(iv0), vertex_ptr(iv1),
                    vertex_ptr(iv2), vertex_ptr(iv3)
                )) == ZERO
        ) {
            ++iv3;
        }

        if(iv3 == nb_vertices()) {
            return false;
        }

        geo_debug_assert(s != ZERO);

        if(s == NEGATIVE) {
            std::swap(iv2, iv3);
        }

        // Create the first tetrahedron
        index_t t0 = new_tetrahedron(
            signed_index_t(iv0), 
            signed_index_t(iv1), 
            signed_index_t(iv2), 
            signed_index_t(iv3)
        );

        // Create the first four virtual tetrahedra surrounding it
        index_t t[4];
        for(index_t f = 0; f < 4; ++f) {
            // In reverse order since it is an adjacent tetrahedron
            signed_index_t v1 = tet_vertex(t0, tet_facet_vertex(f,2));
            signed_index_t v2 = tet_vertex(t0, tet_facet_vertex(f,1));
            signed_index_t v3 = tet_vertex(t0, tet_facet_vertex(f,0));
            t[f] = new_tetrahedron(VERTEX_AT_INFINITY, v1, v2, v3);
        }

        // Connect the virtual tetrahedra to the real one
        for(index_t f=0; f<4; ++f) {
            set_tet_adjacent(t[f], 0, t0);
            set_tet_adjacent(t0, f, t[f]);
        }

        // Interconnect the four virtual tetrahedra along their common
        // faces
        for(index_t f = 0; f < 4; ++f) {
            // In reverse order since it is an adjacent tetrahedron
            index_t lv1 = tet_facet_vertex(f,2);
            index_t lv2 = tet_facet_vertex(f,1);
            index_t lv3 = tet_facet_vertex(f,0);
            set_tet_adjacent(t[f], 1, t[lv1]);
            set_tet_adjacent(t[f], 2, t[lv2]);
            set_tet_adjacent(t[f], 3, t[lv3]);
        }

        return true;
    }

    

    void Delaunay3d::show_tet(index_t t) const {
        std::cerr << "tet"
            << (tet_is_in_list(t) ? '*' : ' ')
            << t
            << ", v=["
            << tet_vertex(t, 0)
            << ' '
            << tet_vertex(t, 1)
            << ' '
            << tet_vertex(t, 2)
            << ' '
            << tet_vertex(t, 3)
            << "]  adj=[";
        show_tet_adjacent(t, 0);
        show_tet_adjacent(t, 1);
        show_tet_adjacent(t, 2);
        show_tet_adjacent(t, 3);
        std::cerr << "] ";

        for(index_t f = 0; f < 4; ++f) {
            std::cerr << 'f' << f << ':';
            for(index_t v = 0; v < 3; ++v) {
                std::cerr << tet_vertex(t, tet_facet_vertex(f,v))
                    << ',';
            }
            std::cerr << ' ';
        }
        std::cerr << std::endl;
    }

    void Delaunay3d::show_tet_adjacent(index_t t, index_t lf) const {
        signed_index_t adj = tet_adjacent(t, lf);
        if(adj != -1) {
            std::cerr << (tet_is_in_list(index_t(adj)) ? '*' : ' ');
        }
        std::cerr << adj;
        std::cerr << ' ';
    }

    void Delaunay3d::show_list(
        index_t first, const std::string& list_name
    ) const {
        index_t t = first;
        std::cerr << "tet list: " << list_name << std::endl;
        while(t != END_OF_LIST) {
            show_tet(t);
            t = tet_next(t);
        }
        std::cerr << "-------------" << std::endl;
    }

    void Delaunay3d::check_combinatorics(bool verbose) const {
        if(verbose) {
            std::cerr << std::endl;
        }
        bool ok = true;
        std::vector<bool> v_has_tet(nb_vertices(), false);
        for(index_t t = 0; t < max_t(); ++t) {
            if(tet_is_free(t)) {
/*
                if(verbose) {
                    std::cerr << "-Deleted tet: ";
                    show_tet(t);
                }
*/
            } else {
/*
                if(verbose) {
                    std::cerr << "Checking tet: ";
                    show_tet(t);
                }
*/
                for(index_t lf = 0; lf < 4; ++lf) {
                    if(tet_adjacent(t, lf) == -1) {
                        std::cerr << lf << ":Missing adjacent tet"
                            << std::endl;
                        ok = false;
                    } else if(tet_adjacent(t, lf) == signed_index_t(t)) {
                        std::cerr << lf << ":Tet is adjacent to itself"
                            << std::endl;
                        ok = false;
                    } else {
                        index_t t2 = index_t(tet_adjacent(t, lf));
                        bool found = false;
                        for(index_t lf2 = 0; lf2 < 4; ++lf2) {
                            if(tet_adjacent(t2, lf2) == signed_index_t(t)) {
                                found = true;
                            }
                        }
                        if(!found) {
                            std::cerr
                                << lf << ":Adjacent link is not bidirectional"
                                << std::endl;
                            ok = false;
                        }
                    }
                }
                index_t nb_infinite = 0;
                for(index_t lv = 0; lv < 4; ++lv) {
                    if(tet_vertex(t, lv) == -1) {
                        ++nb_infinite;
                    }
                }
                if(nb_infinite > 1) {
                    ok = false;
                    std::cerr << "More than one infinite vertex"
                        << std::endl;
                }
            }
            for(index_t lv = 0; lv < 4; ++lv) {
                signed_index_t v = tet_vertex(t, lv);
                if(v >= 0) {
                    v_has_tet[index_t(v)] = true;
                }
            }
        }
        for(index_t v = 0; v < nb_vertices(); ++v) {
            if(!v_has_tet[v]) {
                if(verbose) {
                    std::cerr << "Vertex " << v
                        << " is isolated (duplicated ?)" << std::endl;
                }
            }
        }
        geo_assert(ok);
        if(verbose) {
            std::cerr << std::endl;
        }
        std::cerr << std::endl << "Delaunay Combi OK" << std::endl;
    }

    void Delaunay3d::check_geometry(bool verbose) const {
        bool ok = true;
        for(index_t t = 0; t < max_t(); ++t) {
            if(!tet_is_free(t)) {
                signed_index_t v0 = tet_vertex(t, 0);
                signed_index_t v1 = tet_vertex(t, 1);
                signed_index_t v2 = tet_vertex(t, 2);
                signed_index_t v3 = tet_vertex(t, 3);
                for(index_t v = 0; v < nb_vertices(); ++v) {
                    signed_index_t sv = signed_index_t(v);
                    if(sv == v0 || sv == v1 || sv == v2 || sv == v3) {
                        continue;
                    }
                    if(tet_is_conflict(t, vertex_ptr(v))) {
                        ok = false;
                        if(verbose) {
                            std::cerr << "Tet " << t <<
                                " is in conflict with vertex " << v
                                    << std::endl;

                            std::cerr << "  offending tet: ";
                            show_tet(t);
                        }
                    }
                }
            }
        }
        geo_assert(ok);
        std::cerr << std::endl << "Delaunay Geo OK" << std::endl;
    }

    

    RegularWeightedDelaunay3d::RegularWeightedDelaunay3d(
        coord_index_t dimension
    ) :
        Delaunay3d(4)
    {
        if(dimension != 4) {
            throw InvalidDimension(dimension, "RegularWeightedDelaunay3d", "4");
        }
    }

    RegularWeightedDelaunay3d::~RegularWeightedDelaunay3d() {
    }
}


/******* extracted from parallel_delaunay_3d.cpp *******/

#ifdef GEOGRAM_WITH_PDEL


// ParallelDelaunayThread class, declared locally, has
// no out-of-line virtual functions. It is not a
// problem since they are only visible from this translation
// unit, but clang will complain.
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wweak-vtables"
#endif

namespace {
    using namespace GEO;

    index_t thread_safe_random(index_t choices_in) {
        signed_index_t choices = signed_index_t(choices_in);
        static thread_local long int randomseed = 1l ;
        if (choices >= 714025l) {
            long int newrandom = (randomseed * 1366l + 150889l) % 714025l;
            randomseed = (newrandom * 1366l + 150889l) % 714025l;
            newrandom = newrandom * (choices / 714025l) + randomseed;
            if (newrandom >= choices) {
                return index_t(newrandom - choices);
            } else {
                return index_t(newrandom);
            }
        } else {
            randomseed = (randomseed * 1366l + 150889l) % 714025l;
            return index_t(randomseed % choices);
        }
    }

    index_t thread_safe_random_4() {
        static thread_local long int randomseed = 1l ;
        randomseed = (randomseed * 1366l + 150889l) % 714025l;
        return index_t(randomseed % 4);
    }
}

namespace GEO {

    class Delaunay3dThread : public GEO::Thread {
    public:

        static const index_t NO_THREAD = thread_index_t(-1);

        Delaunay3dThread(
            ParallelDelaunay3d* master,
            index_t pool_begin,
            index_t pool_end
        ) : 
            master_(master),
            cell_to_v_store_(master_->cell_to_v_store_),
            cell_to_cell_store_(master_->cell_to_cell_store_),
            cell_next_(master_->cell_next_),
            cell_thread_(master_->cell_thread_)
        {

            // max_used_t_ is initialized to 1 so that
            // computing modulos does not trigger FPEs
            // at the beginning.
            max_used_t_ = 1;
            max_t_ = master_->cell_next_.size();

            nb_vertices_ = master_->nb_vertices();
            vertices_ = master_->vertex_ptr(0);
            weighted_ = master_->weighted_;
            heights_ = weighted_ ? master_->heights_.data() : nullptr;
            dimension_ = master_->dimension();
            vertex_stride_ = dimension_;
            reorder_ = master_->reorder_.data();

            // Initialize free list in memory pool
            first_free_ = pool_begin;
            for(index_t t=pool_begin; t<pool_end-1; ++t) {
                cell_next_[t] = t+1;
            }
            cell_next_[pool_end-1] = END_OF_LIST;
            nb_free_ = pool_end - pool_begin;
            memory_overflow_ = false;

            work_begin_ = -1;
            work_end_ = -1;
            finished_ = false;
            b_hint_ = NO_TETRAHEDRON;
            e_hint_ = NO_TETRAHEDRON;
            direction_ = true;

#ifdef GEO_DEBUG
            nb_acquired_tets_ = 0;
#endif
            interfering_thread_ = NO_THREAD;

            nb_rollbacks_ = 0;
            nb_failed_locate_ = 0;

            nb_tets_to_create_ = 0;
            t_boundary_ = NO_TETRAHEDRON;
            f_boundary_ = index_t(-1);

            v1_ = index_t(-1);
            v2_ = index_t(-1);
            v3_ = index_t(-1);
            v4_ = index_t(-1);

            pthread_cond_init(&cond_, nullptr);
            pthread_mutex_init(&mutex_, nullptr);
        }

        ~Delaunay3dThread() override {
            pthread_mutex_destroy(&mutex_);
            pthread_cond_destroy(&cond_);
        }

        void initialize_from(const Delaunay3dThread* rhs) {
            max_used_t_ = rhs->max_used_t_;
            max_t_ = rhs->max_t_;
            v1_ = rhs->v1_;
            v2_ = rhs->v2_;
            v3_ = rhs->v3_;
            v4_ = rhs->v4_;
        }

        index_t nb_rollbacks() const {
            return nb_rollbacks_;
        }

        index_t nb_failed_locate() const {
            return nb_failed_locate_;
        }

        void set_work(index_t b, index_t e) {
            work_begin_ = signed_index_t(b);
            // e is one position past the last point index
            // to insert. 
            work_end_ = signed_index_t(e)-1;
        }

        index_t work_size() const {
            if(work_begin_ == -1 && work_end_ == -1) {
                return 0;
            }
            geo_debug_assert(work_begin_ != -1);
            geo_debug_assert(work_end_ != -1);
            return std::max(index_t(work_end_ - work_begin_ + 1),index_t(0));
        }

        index_t nb_threads() const {
            return index_t(master_->threads_.size());
        }

        Delaunay3dThread* thread(index_t t) {
            return static_cast<Delaunay3dThread*>(
                master_->threads_[t].get()
            );
        }

	void run() override {
            
            finished_ = false;

            if(work_begin_ == -1 || work_end_ == -1) {
                return ;
            }

            memory_overflow_ = false;

            // Current hint associated with b
            b_hint_ = NO_TETRAHEDRON;

            // Current hint associated with e
            e_hint_ = NO_TETRAHEDRON;

            // If true, insert in b->e order,
            // else insert in e->b order
            direction_ = true;
            
            while(work_end_ >= work_begin_ && !memory_overflow_) {
                index_t v = direction_ ? 
                    index_t(work_begin_) : index_t(work_end_) ;
                index_t& hint = direction_ ? b_hint_ : e_hint_;

                // Try to insert v and update hint
                bool success = insert(reorder_[v],hint);

                //   Notify all threads that are waiting for
                // this thread to release some tetrahedra.
                send_event();

                if(success) {
                    if(direction_) {
                        ++work_begin_;
                    } else {
                        --work_end_;
                    }
                } else {
                    ++nb_rollbacks_;
                    if(interfering_thread_ != NO_THREAD) {
                        interfering_thread_ = thread_index_t(
                            interfering_thread_ >> 1
                        );
                        if(id() < interfering_thread_) {
                            // If this thread has a higher priority than
                            // the one that interfered, wait for the
                            // interfering thread to release the tets that
                            // it holds (then the loop will retry to insert
                            // the same vertex).
                            wait_for_event(interfering_thread_);
                        } else {
                            // If this thread has a lower priority than
                            // the interfering thread, try inserting 
                            // from the other end of the points sequence.
                            direction_ = !direction_;
                        }
                    }
                }
            }
            finished_ = true;

	    //   Fix by Hiep Vu: wake up threads that potentially missed
	    // the previous wake ups.
	    pthread_mutex_lock(&mutex_);
	    send_event();
	    pthread_mutex_unlock(&mutex_);
        }

        static const index_t NO_TETRAHEDRON = index_t(-1);

        static const signed_index_t VERTEX_AT_INFINITY = -1;
        

        index_t max_t() const {
            return max_t_;
        }

        bool tet_is_finite(index_t t) const {
            return 
                cell_to_v_store_[4 * t]     >= 0 &&
                cell_to_v_store_[4 * t + 1] >= 0 &&
                cell_to_v_store_[4 * t + 2] >= 0 &&
                cell_to_v_store_[4 * t + 3] >= 0;
        }
        
        bool tet_is_real(index_t t) const {
            return !tet_is_free(t) && tet_is_finite(t);
        }

        bool tet_is_free(index_t t) const {
            return tet_is_in_list(t);
        }
        
        bool tet_is_in_list(index_t t, index_t first) const {
            for(
                index_t cur = first; cur != END_OF_LIST; 
                cur = tet_next(cur)
            ) {
                if(cur == t) {
                    return true;
                }
            }
            return false;
        }


        index_t create_first_tetrahedron() {
            index_t iv0,iv1,iv2,iv3;
            if(nb_vertices() < 4) {
                return NO_TETRAHEDRON;
            }

            iv0 = 0;
            
            iv1 = 1;
            while(
                iv1 < nb_vertices() &&
                PCK::points_are_identical_3d(
                    vertex_ptr(iv0), vertex_ptr(iv1)
                    )
                ) {
                ++iv1;
            }
            if(iv1 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }
            
            iv2 = iv1 + 1;
            while(
                iv2 < nb_vertices() &&
                PCK::points_are_colinear_3d(
                    vertex_ptr(iv0), vertex_ptr(iv1), vertex_ptr(iv2)
                    )
                ) {
                ++iv2;
            }
            if(iv2 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }

            iv3 = iv2 + 1;
            Sign s = ZERO;
            while(
                iv3 < nb_vertices() &&
                (s = PCK::orient_3d(
                    vertex_ptr(iv0), vertex_ptr(iv1),
                    vertex_ptr(iv2), vertex_ptr(iv3)
                 )) == ZERO
            ) {
                ++iv3;
            }

            if(iv3 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }

            geo_debug_assert(s != ZERO);
            
            if(s == NEGATIVE) {
                std::swap(iv2, iv3);
            }

            // Create the first tetrahedron
            index_t t0 = new_tetrahedron(
                signed_index_t(iv0), 
                signed_index_t(iv1), 
                signed_index_t(iv2), 
                signed_index_t(iv3)
            );

            // Create the first four virtual tetrahedra surrounding it
            index_t t[4];
            for(index_t f = 0; f < 4; ++f) {
                // In reverse order since it is an adjacent tetrahedron
                signed_index_t v1 = tet_vertex(t0, tet_facet_vertex(f,2));
                signed_index_t v2 = tet_vertex(t0, tet_facet_vertex(f,1));
                signed_index_t v3 = tet_vertex(t0, tet_facet_vertex(f,0));
                t[f] = new_tetrahedron(VERTEX_AT_INFINITY, v1, v2, v3);
            }

            // Connect the virtual tetrahedra to the real one
            for(index_t f=0; f<4; ++f) {
                set_tet_adjacent(t[f], 0, t0);
                set_tet_adjacent(t0, f, t[f]);
            }

            // Interconnect the four virtual tetrahedra along their common
            // faces
            for(index_t f = 0; f < 4; ++f) {
                // In reverse order since it is an adjacent tetrahedron
                index_t lv1 = tet_facet_vertex(f,2);
                index_t lv2 = tet_facet_vertex(f,1);
                index_t lv3 = tet_facet_vertex(f,0);
                set_tet_adjacent(t[f], 1, t[lv1]);
                set_tet_adjacent(t[f], 2, t[lv2]);
                set_tet_adjacent(t[f], 3, t[lv3]);
            }

            v1_ = iv0;
            v2_ = iv1;
            v3_ = iv2;
            v4_ = iv3;

            release_tets();

            return t0;
        }


	index_t stellate_cavity(index_t v) {
	    index_t new_tet = index_t(-1);

	    for(index_t f=0; f<cavity_.nb_facets(); ++f) {
		index_t old_tet = cavity_.facet_tet(f);
		index_t lf = cavity_.facet_facet(f);
		index_t t_neigh = index_t(tet_adjacent(old_tet, lf));
		signed_index_t v1 = cavity_.facet_vertex(f,0);
		signed_index_t v2 = cavity_.facet_vertex(f,1);
		signed_index_t v3 = cavity_.facet_vertex(f,2);
		new_tet = new_tetrahedron(signed_index_t(v), v1, v2, v3);
		set_tet_adjacent(new_tet, 0, t_neigh);
		set_tet_adjacent(t_neigh, find_tet_adjacent(t_neigh,old_tet), new_tet);
		cavity_.set_facet_tet(f, new_tet);
	    }
	
	    for(index_t f=0; f<cavity_.nb_facets(); ++f) {
		new_tet = cavity_.facet_tet(f);
		index_t neigh1, neigh2, neigh3;
		cavity_.get_facet_neighbor_tets(f, neigh1, neigh2, neigh3);
		set_tet_adjacent(new_tet, 1, neigh1);
		set_tet_adjacent(new_tet, 2, neigh2);
		set_tet_adjacent(new_tet, 3, neigh3);		
	    }
	    
	    return new_tet;
	}
	
        bool insert(index_t v, index_t& hint) {

            // If v is one of the vertices of the
            // first tetrahedron, nothing to do.
            if(
                v == v1_ ||
                v == v2_ ||
                v == v3_ ||
                v == v4_
            ) {
                return true;
            }

            Sign orient[4];
            index_t t = locate(vertex_ptr(v),hint,orient);

            //   locate() may fail due to tets already owned by
            // other threads.
            if(t == NO_TETRAHEDRON) {
                ++nb_failed_locate_;
                geo_debug_assert(nb_acquired_tets_ == 0);
                return false;
            }

            //  At this point, t is a valid tetrahedron,
            // and this thread acquired a lock on it.

            // Test whether the point already exists in
            // the triangulation. The point already exists
            // if it's located on three faces of the
            // tetrahedron returned by locate().
            int nb_zero = 
                (orient[0] == ZERO) +
                (orient[1] == ZERO) +
                (orient[2] == ZERO) +
                (orient[3] == ZERO) ;

            if(nb_zero >= 3) {
                release_tet(t);
                return true;
            }

            geo_debug_assert(nb_acquired_tets_ == 1);
            geo_debug_assert(weighted_ || tet_is_in_conflict(t,vertex_ptr(v)));

            index_t t_bndry = NO_TETRAHEDRON;
            index_t f_bndry = index_t(-1);

	    cavity_.clear();
	    
            bool ok = find_conflict_zone(v,t,t_bndry,f_bndry);

            // When in multithreading mode, we cannot allocate memory
            // dynamically and we use a fixed pool. If the fixed pool
            // is full, then we exit the thread (and the missing points
            // are inserted after, in sequential mode).
            if(
                nb_tets_to_create_ > nb_free_ &&
                Process::is_running_threads()
            ) {
                memory_overflow_ = true;
                ok = false;
            }

            if(!ok) {
                //  At this point, this thread did not successfully
                // acquire all the tets in the conflict zone, so
                // we need to rollback.
                release_tets();
                geo_debug_assert(nb_acquired_tets_ == 0);
                return false;
            }

            // The conflict list can be empty if
            //  the triangulation is weighted and v is not visible
            if(tets_to_delete_.size() == 0) {
                release_tets();
                geo_debug_assert(nb_acquired_tets_ == 0);
                return true;
            }


            geo_debug_assert(
                nb_acquired_tets_ == 
                tets_to_delete_.size() + tets_to_release_.size()
            );

#ifdef GEO_DEBUG
            // Sanity check: make sure this threads owns all the tets 
            // in conflict and their neighbors.
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                index_t tdel = tets_to_delete_[i];
                geo_debug_assert(owns_tet(tdel));
                for(index_t lf=0; lf<4; ++lf) {
                    geo_debug_assert(tet_adjacent(tdel,lf) >= 0);
                    geo_debug_assert(owns_tet(index_t(tet_adjacent(tdel,lf))));
                }
            }
#endif
            geo_debug_assert(owns_tet(t_bndry));
            geo_debug_assert(owns_tet(index_t(tet_adjacent(t_bndry,f_bndry))));
            geo_debug_assert(
                !tet_is_marked_as_conflict(
                    index_t(tet_adjacent(t_bndry,f_bndry))
                )
            );

            //   At this point, this threads owns all the tets in conflict and
            // their neighbors, therefore no other thread can interfere, and
            // we can update the triangulation.

	    index_t new_tet = index_t(-1);
	    if(cavity_.OK()) {
		new_tet = stellate_cavity(v);
	    } else {
		new_tet = stellate_conflict_zone_iterative(v,t_bndry,f_bndry);
	    }

       
            // Recycle the tetrahedra of the conflict zone.
            for(index_t i=0; i<tets_to_delete_.size()-1; ++i) {
                cell_next_[tets_to_delete_[i]] = tets_to_delete_[i+1];
            }
            cell_next_[tets_to_delete_[tets_to_delete_.size()-1]] =
                first_free_;
            first_free_ = tets_to_delete_[0];
            nb_free_ += nb_tets_in_conflict();

            // For debugging purposes.
#ifdef GEO_DEBUG
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                index_t tdel = tets_to_delete_[i];
                set_tet_vertex(tdel,0,-2);
                set_tet_vertex(tdel,1,-2);
                set_tet_vertex(tdel,2,-2);
                set_tet_vertex(tdel,3,-2);
            }
#endif
       
            // Return one of the newly created tets
            hint=new_tet;

            release_tets();

            geo_debug_assert(nb_acquired_tets_ == 0);
            return true;
        }

        bool find_conflict_zone(
            index_t v, index_t t, 
            index_t& t_bndry, index_t& f_bndry
        ) {
            nb_tets_to_create_ = 0;

            geo_debug_assert(t != NO_TETRAHEDRON);
            geo_debug_assert(owns_tet(t));

            // Pointer to the coordinates of the point to be inserted
            const double* p = vertex_ptr(v);

            //  Weighted triangulations can have dangling
            // vertices. Such vertices p are characterized by
            // the fact that p is not in conflict with the 
            // tetrahedron returned by locate().
            if(weighted_ && !tet_is_in_conflict(t,p)) {
                release_tet(t);
                return true;
            }

            mark_tet_as_conflict(t);

            //   Sanity check: the vertex to be inserted should
            // not correspond to one of the vertices of t.
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,0));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,1));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,2));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,3));

            // Note: points on edges and on facets are
            // handled by the way tet_is_in_conflict()
            // is implemented, that naturally inserts
            // the correct tetrahedra in the conflict list.

            // Determine the conflict list by greedy propagation from t.
            bool result = find_conflict_zone_iterative(p,t);
            t_bndry = t_boundary_;
            f_bndry = f_boundary_;
            return result;
        }


        bool find_conflict_zone_iterative(
            const double* p, index_t t_in
        ) {
            geo_debug_assert(owns_tet(t_in));
            S_.push_back(t_in);

            while(S_.size() != 0) {
                index_t t = *(S_.rbegin());
                S_.pop_back();

                geo_debug_assert(owns_tet(t));

                for(index_t lf = 0; lf < 4; ++lf) {
                    index_t t2 = index_t(tet_adjacent(t, lf));
                
                    // If t2 is already owned by current thread, then
                    // its status was previously determined.
                    if(owns_tet(t2)) {
                        geo_debug_assert(
                            tet_is_marked_as_conflict(t2) == 
                            tet_is_in_conflict(t2,p)
                        );
                    
                        // If t2 is not in conflict list, then t has a facet
                        // on the border of the conflict zone, and there is
                        // a tet to create.
                        if(!tet_is_marked_as_conflict(t2)) {
                            ++nb_tets_to_create_;
			    cavity_.new_facet(
				t, lf,
				tet_vertex(t, tet_facet_vertex(lf,0)),
				tet_vertex(t, tet_facet_vertex(lf,1)),
				tet_vertex(t, tet_facet_vertex(lf,2))
			    );
                        }
                        continue;
                    }

                    if(!acquire_tet(t2)) {
                        S_.resize(0);
                        return false;
                    }
                
                    geo_debug_assert(owns_tet(t2));

                    if(!tet_is_in_conflict(t2,p)) {
                        mark_tet_as_neighbor(t2);
                        // If t2 is not in conflict list, then t has a facet
                        // on the border of the conflict zone, and there is
                        // a tet to create.
                        ++nb_tets_to_create_;
                    } else {
                        mark_tet_as_conflict(t2);
                        geo_debug_assert(owns_tet(t2));
                        S_.push_back(t2);
                        continue;
                    }

                    //  At this point, t is in conflict 
                    // and t2 is not in conflict. 
                    // We keep a reference to a tet on the boundary
                    t_boundary_ = t;
                    f_boundary_ = lf;
                    ++nb_tets_to_create_;
		    cavity_.new_facet(
			t, lf,
			tet_vertex(t, tet_facet_vertex(lf,0)),
			tet_vertex(t, tet_facet_vertex(lf,1)),
			tet_vertex(t, tet_facet_vertex(lf,2))
		    );
                    geo_debug_assert(tet_adjacent(t,lf) == signed_index_t(t2));
                    geo_debug_assert(owns_tet(t));
                    geo_debug_assert(owns_tet(t2));
                }
            }
            return true;
        }

        double lifted_coordinate(const double* p) const {
            // Compute the index of the point from its address
            index_t pindex = index_t(
                (p - vertex_ptr(0)) / int(vertex_stride_)
            );
            return heights_[pindex];
        }
        
        
        bool tet_is_in_conflict(index_t t, const double* p) const {

            // Lookup tetrahedron vertices
            const double* pv[4];
            for(index_t i=0; i<4; ++i) {
                signed_index_t v = tet_vertex(t,i);
                pv[i] = (v == -1) ? nullptr : vertex_ptr(index_t(v));
            }

            // Check for virtual tetrahedra (then in_sphere()
            // is replaced with orient3d())
            for(index_t lf = 0; lf < 4; ++lf) {

                if(pv[lf] == nullptr) {

                    // Facet of a virtual tetrahedron opposite to
                    // infinite vertex corresponds to
                    // the triangle on the convex hull of the points.
                    // Orientation is obtained by replacing vertex lf
                    // with p.
                    pv[lf] = p;
                    Sign sign = PCK::orient_3d(pv[0],pv[1],pv[2],pv[3]);

                    if(sign > 0) {
                        return true;
                    }

                    if(sign < 0) {
                        return false;
                    }

                    // If sign is zero, we check the real tetrahedron
                    // adjacent to the facet on the convex hull.
                    geo_debug_assert(tet_adjacent(t, lf) >= 0);
                    index_t t2 = index_t(tet_adjacent(t, lf));
                    geo_debug_assert(!tet_is_virtual(t2));

                    //   If t2 was already visited by this thread, then
                    // it is in conflict if it is already marked.
                    if(owns_tet(t2)) {
                        return tet_is_marked_as_conflict(t2);
                    }
                    
                    //  If t2 was not already visited, then we need to
                    // switch to the in_circum_circle_3d() predicate.

                    const double* q0 = pv[(lf+1)%4];
                    const double* q1 = pv[(lf+2)%4];
                    const double* q2 = pv[(lf+3)%4];
                    
                    if(weighted_) {
                        return (
                            PCK::in_circle_3dlifted_SOS(
                                q0, q1, q2, p,
                                lifted_coordinate(q0),
                                lifted_coordinate(q1),
                                lifted_coordinate(q2),
                                lifted_coordinate(p)
                            ) > 0
                        );
                    } else {
                        return (
                            PCK::in_circle_3d_SOS(
                                q0,q1,q2,p
                            ) > 0
                        );
                    }
                }
            }

            //   If the tetrahedron is a finite one, it is in conflict
            // if its circumscribed sphere contains the point (this is
            // the standard case).

            if(weighted_) {
                double h0 = heights_[finite_tet_vertex(t, 0)];
                double h1 = heights_[finite_tet_vertex(t, 1)];
                double h2 = heights_[finite_tet_vertex(t, 2)];
                double h3 = heights_[finite_tet_vertex(t, 3)];
                double h = lifted_coordinate(p);
                return (PCK::orient_3dlifted_SOS(
                            pv[0],pv[1],pv[2],pv[3],p,h0,h1,h2,h3,h
                       ) > 0) ;
            }

            return (PCK::in_sphere_3d_SOS(pv[0], pv[1], pv[2], pv[3], p) > 0);
        }
        

         index_t locate(
            const double* p, index_t hint = NO_TETRAHEDRON,
            Sign* orient = nullptr
         ) {
             //   Try improving the hint by using the 
             // inexact locate function. This gains
             // (a little bit) performance (a few 
             // percent in total Delaunay computation
             // time), but it is better than nothing...
             //   Note: there is a maximum number of tets 
             // traversed by locate_inexact()  (2500)
             // since there exists configurations in which
             // locate_inexact() loops forever !

             {
                 index_t new_hint = locate_inexact(p, hint, 2500);

                 if(new_hint == NO_TETRAHEDRON) {
                     return NO_TETRAHEDRON;
                 }

                 hint = new_hint;
             }

             // If no hint specified, find a tetrahedron randomly

             if(hint != NO_TETRAHEDRON) {
                 if(tet_is_free(hint)) {
                     hint = NO_TETRAHEDRON;
                 } else {
                     if( !owns_tet(hint) && !acquire_tet(hint) ) {
                         hint = NO_TETRAHEDRON;
                     }
                     if((hint != NO_TETRAHEDRON) && tet_is_free(hint)) {
                         release_tet(hint);
                         hint = NO_TETRAHEDRON;
                     }
                 }
             }

             do {
                 if(hint == NO_TETRAHEDRON) {
                     hint = thread_safe_random(max_used_t_);
                 }
                 if(
                     tet_is_free(hint) || 
                     (!owns_tet(hint) && !acquire_tet(hint))
                 ) {
                     if(owns_tet(hint)) {
                         release_tet(hint);
                     }
                     hint = NO_TETRAHEDRON;
                 } else {
                     for(index_t f=0; f<4; ++f) {
                         if(tet_vertex(hint,f) == VERTEX_AT_INFINITY) {
                             index_t new_hint = index_t(tet_adjacent(hint,f));
                             if(
                                 tet_is_free(new_hint) || 
                                 !acquire_tet(new_hint)
                             ) {
                                 new_hint = NO_TETRAHEDRON;
                             }
                             release_tet(hint);
                             hint = new_hint;
                             break;
                         }
                     }
                 }
             } while(hint == NO_TETRAHEDRON) ;

             index_t t = hint;
             index_t t_pred = NO_TETRAHEDRON;
             Sign orient_local[4];
             if(orient == nullptr) {
                 orient = orient_local;
             }


         still_walking:
             {
                 if(t_pred != NO_TETRAHEDRON) {
                     release_tet(t_pred);
                 }

                 if(tet_is_free(t)) {
                     return NO_TETRAHEDRON;
                 }

                 if(!owns_tet(t) && !acquire_tet(t)) {
                     return NO_TETRAHEDRON;
                 }


                 if(!tet_is_real(t)) {
                     release_tet(t);
                     return NO_TETRAHEDRON;
                 }

                 const double* pv[4];
                 pv[0] = vertex_ptr(finite_tet_vertex(t,0));
                 pv[1] = vertex_ptr(finite_tet_vertex(t,1));
                 pv[2] = vertex_ptr(finite_tet_vertex(t,2));
                 pv[3] = vertex_ptr(finite_tet_vertex(t,3));
                 
                 // Start from a random facet
                 index_t f0 = thread_safe_random_4();
                 for(index_t df = 0; df < 4; ++df) {
                     index_t f = (f0 + df) % 4;
                     
                     signed_index_t s_t_next = tet_adjacent(t,f);
                     
                     //  If the opposite tet is -1, then it means that
                     // we are trying to locate() (e.g. called from
                     // nearest_vertex) within a tetrahedralization 
                     // from which the infinite tets were removed.
                     if(s_t_next == -1) {
                         release_tet(t);
                         return NO_TETRAHEDRON;
                     }
                     
                     index_t t_next = index_t(s_t_next);
                     
                     //   If the candidate next tetrahedron is the
                     // one we came from, then we know already that
                     // the orientation is positive, thus we examine
                     // the next candidate (or exit the loop if they
                     // are exhausted).
                     if(t_next == t_pred) {
                         orient[f] = POSITIVE ;
                         continue ; 
                     }

                     //   To test the orientation of p w.r.t. the facet f of
                     // t, we replace vertex number f with p in t (same
                     // convention as in CGAL).
                     // This is equivalent to tet_facet_point_orient3d(t,f,p)
                     // (but less costly, saves a couple of lookups)
                     const double* pv_bkp = pv[f];
                     pv[f] = p;
                     orient[f] = PCK::orient_3d(pv[0], pv[1], pv[2], pv[3]);
                     
                     //   If the orientation is not negative, then we cannot
                     // walk towards t_next, and examine the next candidate
                     // (or exit the loop if they are exhausted).
                     if(orient[f] != NEGATIVE) {
                         pv[f] = pv_bkp;
                         continue;
                     }

                     //  If the opposite tet is a virtual tet, then
                     // the point has a positive orientation relative
                     // to the facet on the border of the convex hull,
                     // thus t_next is a tet in conflict and we are
                     // done.
                     if(tet_is_virtual(t_next)) {
                         release_tet(t);
                         if(!acquire_tet(t_next)) {
                             return NO_TETRAHEDRON;
                         }
                         for(index_t lf = 0; lf < 4; ++lf) {
                             orient[lf] = POSITIVE;
                         }
                         return t_next;
                     }
                     
                     //   If we reach this point, then t_next is a valid
                     // successor, thus we are still walking.
                     t_pred = t;
                     t = t_next;
                     goto still_walking;
                 }
             } 

             //   If we reach this point, we did not find a valid successor
             // for walking (a face for which p has negative orientation), 
             // thus we reached the tet for which p has all positive 
             // face orientations (i.e. the tet that contains p).

#ifdef GEO_DEBUG
             geo_debug_assert(tet_is_real(t));

             const double* pv[4];
             Sign signs[4];
             pv[0] = vertex_ptr(finite_tet_vertex(t,0));
             pv[1] = vertex_ptr(finite_tet_vertex(t,1));
             pv[2] = vertex_ptr(finite_tet_vertex(t,2));
             pv[3] = vertex_ptr(finite_tet_vertex(t,3));
             for(index_t f=0; f<4; ++f) {
                 const double* pv_bkp = pv[f];
                 pv[f] = p;
                 signs[f] = PCK::orient_3d(pv[0], pv[1], pv[2], pv[3]);
                 geo_debug_assert(signs[f] >= 0);
                 pv[f] = pv_bkp;
             }
#endif

             return t;
         }
        

    protected:
        
        bool tet_is_marked_as_conflict(index_t t) const {
            geo_debug_assert(owns_tet(t));
            return ((cell_thread_[t] & 1) != 0);
        }


        index_t nb_tets_in_conflict() const {
            return tets_to_delete_.size();
        }

        void mark_tet_as_conflict(index_t t) {
            geo_debug_assert(owns_tet(t));
            tets_to_delete_.push_back(t);
            cell_thread_[t] |= 1;
            geo_debug_assert(owns_tet(t));
            geo_debug_assert(tet_is_marked_as_conflict(t));
        }

        void mark_tet_as_neighbor(index_t t) {
            //   Note: nothing to change in cell_thread_[t]
            // since LSB=0 means neigbhor tet.
            tets_to_release_.push_back(t);
        }

        void acquire_and_mark_tet_as_created(index_t t) {
            //  The tet was created in this thread's tet pool,
            // therefore there is no need to use sync 
            // primitives to acquire a lock on it.
            geo_debug_assert(cell_thread_[t] == NO_THREAD);
            cell_thread_[t] = thread_index_t(id() << 1);
#ifdef GEO_DEBUG
            ++nb_acquired_tets_;
#endif
            tets_to_release_.push_back(t);            
        }


        void release_tets() {
            for(index_t i=0; i<tets_to_release_.size(); ++i) {
                release_tet(tets_to_release_[i]);
            }
            tets_to_release_.resize(0);
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                release_tet(tets_to_delete_[i]);
            }
            tets_to_delete_.resize(0);
        }

        bool acquire_tet(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!owns_tet(t));

#if defined(GEO_COMPILER_MSVC) 
           // Note: comparand and exchange parameter are swapped in Windows API
           // as compared to __sync_val_compare_and_swap !!
            interfering_thread_ =
                (thread_index_t)(_InterlockedCompareExchange8(
                    (volatile char *)(&cell_thread_[t]),
                    (char)(id() << 1),
                    (char)(NO_THREAD)
                ));
#else            
            interfering_thread_ = 
                __sync_val_compare_and_swap(
                    &cell_thread_[t], NO_THREAD, thread_index_t(id() << 1)
                );
#endif
            
            if(interfering_thread_ == NO_THREAD) {
                geo_debug_assert(t == first_free_ || !tet_is_in_list(t));
#ifdef GEO_DEBUG
                ++nb_acquired_tets_;
#endif
                return true;
            }
            return false;
        }

        void release_tet(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(owns_tet(t));
#ifdef GEO_DEBUG
            --nb_acquired_tets_;
#endif
            cell_thread_[t] = NO_THREAD;
        }


        bool owns_tet(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_thread_[t] >> 1) == thread_index_t(id());
        }

         index_t locate_inexact(
             const double* p, index_t hint, index_t max_iter
         ) const {
             // If no hint specified, find a tetrahedron randomly
             while(hint == NO_TETRAHEDRON) {
                 hint = thread_safe_random(max_used_t_);
                 if(tet_is_free(hint) || tet_thread(hint) != NO_THREAD) {
                     hint = NO_TETRAHEDRON;
                 }
             }

             //  Always start from a real tet. If the tet is virtual,
             // find its real neighbor (always opposite to the
             // infinite vertex)
             if(tet_is_virtual(hint)) {
                 for(index_t lf = 0; lf < 4; ++lf) {
                     if(tet_vertex(hint, lf) == VERTEX_AT_INFINITY) {
                         hint = index_t(tet_adjacent(hint, lf));

                         // Yes, this can happen if the tetrahedron was
                         // modified by another thread in the meanwhile.
                         if(hint == NO_TETRAHEDRON) {
                             return NO_TETRAHEDRON;
                         }
                         
                         break;
                     }
                 }
             }

             index_t t = hint;
             index_t t_pred = NO_TETRAHEDRON;
             
         still_walking:
             {

                 // Lookup the vertices of the current tetrahedron.
                 const double* pv[4];
                 for(index_t lv=0; lv<4; ++lv) {
                     signed_index_t iv = tet_vertex(t,lv);

                     // Since we did not acquire any lock,
                     // it is possible that another threads made
                     // this tetrahedron virtual (in this case
                     // we exit immediately).
                     if(iv < 0) {
                         return NO_TETRAHEDRON;
                     }
                     pv[lv] = vertex_ptr(index_t(iv));
                 }

                 for(index_t f = 0; f < 4; ++f) {
                     
                     signed_index_t s_t_next = tet_adjacent(t,f);
                     
                     //  If the opposite tet is -1, then it means that
                     // we are trying to locate() (e.g. called from
                     // nearest_vertex) within a tetrahedralization 
                     // from which the infinite tets were removed.
                     if(s_t_next == -1) {
                         return NO_TETRAHEDRON;
                     }

                     index_t t_next = index_t(s_t_next);
                     
                     //   If the candidate next tetrahedron is the
                     // one we came from, then we know already that
                     // the orientation is positive, thus we examine
                     // the next candidate (or exit the loop if they
                     // are exhausted).
                     if(t_next == t_pred) {
                         continue ; 
                     }
                     
                     //   To test the orientation of p w.r.t. the facet f of
                     // t, we replace vertex number f with p in t (same
                     // convention as in CGAL).
                     const double* pv_bkp = pv[f];
                     pv[f] = p;
                     Sign ori = PCK::orient_3d_inexact(pv[0], pv[1], pv[2], pv[3]);
                     
                     //   If the orientation is not negative, then we cannot
                     // walk towards t_next, and examine the next candidate
                     // (or exit the loop if they are exhausted).
                     if(ori != NEGATIVE) {
                         pv[f] = pv_bkp;
                         continue;
                     }

                     //  If the opposite tet is a virtual tet, then
                     // the point has a positive orientation relative
                     // to the facet on the border of the convex hull,
                     // thus t_next is a tet in conflict and we are
                     // done.
                     if(tet_is_virtual(t_next)) {
                         return t_next;
                     }

                     //   If we reach this point, then t_next is a valid
                     // successor, thus we are still walking.
                     t_pred = t;
                     t = t_next;
                     if(--max_iter != 0) {
                         goto still_walking;
                     }
                 }
             } 

             //   If we reach this point, we did not find a valid successor
             // for walking (a face for which p has negative orientation), 
             // thus we reached the tet for which p has all positive 
             // face orientations (i.e. the tet that contains p).

             return t;
         }


        bool tet_is_virtual(index_t t) const {
            return
                !tet_is_free(t) && (
                cell_to_v_store_[4 * t] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 1] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 2] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 3] == VERTEX_AT_INFINITY) ;
        }

        
        static index_t tet_facet_vertex(index_t f, index_t v) {
            geo_debug_assert(f < 4);
            geo_debug_assert(v < 3);
            return index_t(tet_facet_vertex_[f][v]);
        }

        signed_index_t tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            return cell_to_v_store_[4 * t + lv];
        }

        index_t find_tet_vertex(index_t t, signed_index_t v) const {
            geo_debug_assert(t < max_t());
            //   Find local index of v in tetrahedron t vertices.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);
            return find_4(T,v);
        }


         index_t finite_tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            geo_debug_assert(cell_to_v_store_[4 * t + lv] != -1);
            return index_t(cell_to_v_store_[4 * t + lv]);
        }

        void set_tet_vertex(index_t t, index_t lv, signed_index_t v) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            geo_debug_assert(owns_tet(t));
            cell_to_v_store_[4 * t + lv] = v;
        }

        signed_index_t tet_adjacent(index_t t, index_t lf) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lf < 4);
            signed_index_t result = cell_to_cell_store_[4 * t + lf];
            return result;
        }

        void set_tet_adjacent(index_t t1, index_t lf1, index_t t2) {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2 < max_t());
            geo_debug_assert(lf1 < 4);
            geo_debug_assert(owns_tet(t1));
            geo_debug_assert(owns_tet(t2));
            cell_to_cell_store_[4 * t1 + lf1] = signed_index_t(t2);
        }
        
        index_t find_tet_adjacent(
            index_t t1, index_t t2_in
        ) const {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2_in < max_t());
            geo_debug_assert(t1 != t2_in);

            signed_index_t t2 = signed_index_t(t2_in);

            // Find local index of t2 in tetrahedron t1 adajcent tets.
            const signed_index_t* T = &(cell_to_cell_store_[4 * t1]);
            index_t result = find_4(T,t2);

            // Sanity check: make sure that t1 is adjacent to t2
            // only once!
            geo_debug_assert(tet_adjacent(t1,(result+1)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+2)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+3)%4) != t2);
            return result;
        }


        index_t get_facet_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);
            //   Find local index of v1 and v2 in tetrahedron t
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);
            index_t lv1 = find_4(T,v1);
            index_t lv2 = find_4(T,v2);
            geo_debug_assert(lv1 != lv2);
            return index_t(halfedge_facet_[lv1][lv2]);
        }
        

        void get_facets_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2,
            index_t& f12, index_t& f21
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);
        
            //   Find local index of v1 and v2 in tetrahedron t
            // The following expression is 10% faster than using
            // if() statements (multiply by boolean result of test).
            // Thank to Laurent Alonso for this idea.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);

            signed_index_t lv1 = 
                (T[1] == v1) | ((T[2] == v1) * 2) | ((T[3] == v1) * 3);
            
            signed_index_t lv2 = 
                (T[1] == v2) | ((T[2] == v2) * 2) | ((T[3] == v2) * 3);
            
            geo_debug_assert(lv1 != 0 || T[0] == v1);
            geo_debug_assert(lv2 != 0 || T[0] == v2);
            geo_debug_assert(lv1 >= 0);
            geo_debug_assert(lv2 >= 0);
            geo_debug_assert(lv1 != lv2);

            f12 = index_t(halfedge_facet_[lv1][lv2]);
            f21 = index_t(halfedge_facet_[lv2][lv1]);
        }

        static const index_t END_OF_LIST = index_t(-1);


        static const index_t NOT_IN_LIST = index_t(-2);

        index_t nb_vertices() const {
            return nb_vertices_;
        }

        const double* vertex_ptr(index_t i) const {
            geo_debug_assert(i < nb_vertices());
            return vertices_ + vertex_stride_ * i;
        }

        bool tet_is_in_list(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_next_[t] != NOT_IN_LIST);
        }

        index_t tet_next(index_t t) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            return cell_next_[t];
        }


        index_t tet_thread(index_t t) const {
            geo_debug_assert(t < max_t());
            return cell_thread_[t];
        }

        void add_tet_to_list(index_t t, index_t& first, index_t& last) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!tet_is_in_list(t));
            geo_debug_assert(owns_tet(t));
            if(last == END_OF_LIST) {
                geo_debug_assert(first == END_OF_LIST);
                first = last = t;
                cell_next_[t] = END_OF_LIST;
            } else {
                cell_next_[t] = first;
                first = t;
            }
        }

        void remove_tet_from_list(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            geo_debug_assert(owns_tet(t));
            cell_next_[t] = NOT_IN_LIST;
        }


        index_t new_tetrahedron() {

            // If the memory pool is full, then we expand it.
            // This cannot be done when running multiple threads.
            if(first_free_ == END_OF_LIST) {
                geo_debug_assert(!Process::is_running_threads());
                master_->cell_to_v_store_.resize(
                    master_->cell_to_v_store_.size() + 4, -1
                );
                master_->cell_to_cell_store_.resize(
                    master_->cell_to_cell_store_.size() + 4, -1
                );
                // index_t(NOT_IN_LIST) is necessary, else with
                // NOT_IN_LIST alone the compiler tries to generate a
                // reference to NOT_IN_LIST resulting in a link error.
                master_->cell_next_.push_back(index_t(END_OF_LIST));
                master_->cell_thread_.push_back(thread_index_t(NO_THREAD));
                ++nb_free_;
                ++max_t_;
                first_free_ = master_->cell_thread_.size() - 1;
            }

            acquire_and_mark_tet_as_created(first_free_);
            index_t result = first_free_;

            first_free_ = tet_next(first_free_);
            remove_tet_from_list(result);

            cell_to_cell_store_[4 * result] = -1;
            cell_to_cell_store_[4 * result + 1] = -1;
            cell_to_cell_store_[4 * result + 2] = -1;
            cell_to_cell_store_[4 * result + 3] = -1;

            max_used_t_ = std::max(max_used_t_, result);

            --nb_free_;
            return result;
        }

        index_t new_tetrahedron(
            signed_index_t v1, signed_index_t v2, 
            signed_index_t v3, signed_index_t v4
        ) {
            index_t result = new_tetrahedron();
            cell_to_v_store_[4 * result] = v1;
            cell_to_v_store_[4 * result + 1] = v2;
            cell_to_v_store_[4 * result + 2] = v3;
            cell_to_v_store_[4 * result + 3] = v4;
            return result;
        }

        static index_t find_4(const signed_index_t* T, signed_index_t v) {
            // The following expression is 10% faster than using
            // if() statements. This uses the C++ norm, that 
            // ensures that the 'true' boolean value converted to 
            // an int is always 1. With most compilers, this avoids 
            // generating branching instructions.
            // Thank to Laurent Alonso for this idea.
            // Note: Laurent also has this version:
            //    (T[0] != v)+(T[2]==v)+2*(T[3]==v)
            // that avoids a *3 multiply, but it is not faster in
            // practice.
            index_t result = index_t(
                (T[1] == v) | ((T[2] == v) * 2) | ((T[3] == v) * 3)
            );
            // Sanity check, important if it was T[0], not explicitly
            // tested (detects input that does not meet the precondition).
            geo_debug_assert(T[result] == v);
            return result; 
        }

        void send_event() {
            pthread_cond_broadcast(&cond_);
        }
        
        void wait_for_event(index_t t) {
	    // Fixed by Hiep Vu: enlarged critical section (contains
	    // now the test (!thrd->finished)
            Delaunay3dThread* thrd = thread(t);
	    pthread_mutex_lock(&(thrd->mutex_));	    
            if(!thrd->finished_) {
                pthread_cond_wait(&(thrd->cond_), &(thrd->mutex_));
            }
	    pthread_mutex_unlock(&(thrd->mutex_));	    
        }

                

        class StellateConflictStack {
        public:

            void push(index_t t1, index_t t1fbord, index_t t1fprev) {
                store_.resize(store_.size()+1);
                top().t1 = t1;
                top().t1fbord = Numeric::uint8(t1fbord);
                top().t1fprev = Numeric::uint8(t1fprev);
            }

            void save_locals(index_t new_t, index_t t1ft2, index_t t2ft1) {
                geo_debug_assert(!empty());
                top().new_t = new_t;
                top().t1ft2 = Numeric::uint8(t1ft2);
                top().t2ft1 = Numeric::uint8(t2ft1);
            }

            void get_parameters(
                index_t& t1, index_t& t1fbord, index_t& t1fprev
            ) const {
                geo_debug_assert(!empty());
                t1      = top().t1;
                t1fbord = index_t(top().t1fbord);
                t1fprev = index_t(top().t1fprev);
            }


            void get_locals(
                index_t& new_t, index_t& t1ft2, index_t& t2ft1
            ) const {
                geo_debug_assert(!empty());
                new_t = top().new_t;
                t1ft2 = index_t(top().t1ft2);
                t2ft1 = index_t(top().t2ft1);
            }
            
            void pop() {
                geo_debug_assert(!empty());
                store_.pop_back();
            }
            
            bool empty() const {
                return store_.empty();
            }
            
        private:

            struct Frame {
                // Parameters
                index_t t1;
                index_t new_t;                
                Numeric::uint8 t1fbord ;
                
                // Local variables
                Numeric::uint8 t1fprev ;
                Numeric::uint8 t1ft2   ;
                Numeric::uint8 t2ft1   ;
            };

            Frame& top() {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }

            const Frame& top() const {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }
            
            std::vector<Frame> store_;
        };

        index_t stellate_conflict_zone_iterative(
            index_t v_in, index_t t1, index_t t1fbord,
            index_t t1fprev = index_t(-1)
        ) {
            //   This function is de-recursified because some degenerate
            // inputs can cause stack overflow (system stack is limited to
            // a few megs). For instance, it can happen when a large number
            // of points are on the same sphere exactly.
            
            //   To de-recursify, it uses class StellateConflictStack
            // that emulates system's stack for storing functions's
            // parameters and local variables in all the nested stack
            // frames. 
        
            signed_index_t v = signed_index_t(v_in);
            
            S2_.push(t1, t1fbord, t1fprev);
            
            index_t new_t;   // the newly created tetrahedron.
            
            index_t t1ft2;   // traverses the 4 facets of t1.
            
            index_t t2;      // the tetrahedron on the border of
                             // the conflict zone that shares an
                             // edge with t1 along t1ft2.
            
            index_t t2fbord; // the facet of t2 on the border of
                             // the conflict zone.
        
            index_t t2ft1;   // the facet of t2 that is incident to t1.
        
        entry_point:
            S2_.get_parameters(t1, t1fbord, t1fprev);


            geo_debug_assert(owns_tet(t1));
            geo_debug_assert(tet_adjacent(t1,t1fbord)>=0);
            geo_debug_assert(owns_tet(index_t(tet_adjacent(t1,t1fbord))));
            geo_debug_assert(tet_is_marked_as_conflict(t1));
            geo_debug_assert(
                !tet_is_marked_as_conflict(index_t(tet_adjacent(t1,t1fbord)))
            );

            // Create new tetrahedron with same vertices as t_bndry
            new_t = new_tetrahedron(
                tet_vertex(t1,0),
                tet_vertex(t1,1),
                tet_vertex(t1,2),
                tet_vertex(t1,3)
            );

            // Replace in new_t the vertex opposite to t1fbord with v
            set_tet_vertex(new_t, t1fbord, v);
            
            // Connect new_t with t1's neighbor accros t1fbord
            {
                index_t tbord = index_t(tet_adjacent(t1,t1fbord));
                set_tet_adjacent(new_t, t1fbord, tbord);
                set_tet_adjacent(tbord, find_tet_adjacent(tbord,t1), new_t);
            }
            
            //  Lookup new_t's neighbors accros its three other
            // facets and connect them
            for(t1ft2=0; t1ft2<4; ++t1ft2) {
                
                if(t1ft2 == t1fprev || tet_adjacent(new_t,t1ft2) != -1) {
                    continue;
                }
                
                // Get t1's neighbor along the border of the conflict zone
                if(!get_neighbor_along_conflict_zone_border(
                       t1,t1fbord,t1ft2, t2,t2fbord,t2ft1
                )) {
                    //   If t1's neighbor is not a new tetrahedron,
                    // create a new tetrahedron through a recursive call.
                    S2_.save_locals(new_t, t1ft2, t2ft1);
                    S2_.push(t2, t2fbord, t2ft1);
                    goto entry_point;

                return_point:
                    // This is the return value of the called function.
                    index_t result = new_t;
                    S2_.pop();
                    
                    // Special case: we were in the outermost frame, 
                    // then we (truly) return from the function.
                    if(S2_.empty()) {
                        return result;
                    }
                    
                    S2_.get_parameters(t1, t1fbord, t1fprev);
                    S2_.get_locals(new_t, t1ft2, t2ft1); 
                    t2 = result; 
                }
                
                set_tet_adjacent(t2, t2ft1, new_t);
                set_tet_adjacent(new_t, t1ft2, t2);
            }

            // Except for the initial call (see "Special case" above),
            // the nested calls all come from the same location,
            // thus there is only one possible return point
            // (no need to push any return address).
            goto return_point;
        }

        bool get_neighbor_along_conflict_zone_border(
            index_t t1,
            index_t t1fborder,
            index_t t1ft2,
            index_t& t2,
            index_t& t2fborder,
            index_t& t2ft1
        ) const {
            
            // Note: this function is a bit long for an inline function,
            // but I observed a (modest) performance gain doing so.
            
            //   Find two vertices that are both on facets new_f and f1
            //  (the edge around which we are turning)
            //  This uses duality as follows:
            //  Primal form (not used here): 
            //    halfedge_facet_[v1][v2] returns a facet that is incident
            //    to both v1 and v2.
            //  Dual form (used here):
            //    halfedge_facet_[f1][f2] returns a vertex that both 
            //    f1 and f2 are incident to.
            signed_index_t ev1 = 
                tet_vertex(t1, index_t(halfedge_facet_[t1ft2][t1fborder]));
            signed_index_t ev2 = 
                tet_vertex(t1, index_t(halfedge_facet_[t1fborder][t1ft2]));
            
            //   Turn around edge [ev1,ev2] inside the conflict zone
            // until we reach again the boundary of the conflict zone.
            // Traversing inside the conflict zone is faster (as compared
            // to outside) since it traverses a smaller number of tets.
            index_t cur_t = t1;
            index_t cur_f = t1ft2;
            index_t next_t = index_t(tet_adjacent(cur_t,cur_f));
            while(tet_is_marked_as_conflict(next_t)) {            
                geo_debug_assert(next_t != t1);
                cur_t = next_t;
                cur_f = get_facet_by_halfedge(cur_t,ev1,ev2);
                next_t = index_t(tet_adjacent(cur_t, cur_f));
            }
             
            //  At this point, cur_t is in conflict zone and
            // next_t is outside the conflict zone.
            index_t f12,f21;
            get_facets_by_halfedge(next_t, ev1, ev2, f12, f21);
            t2 = index_t(tet_adjacent(next_t,f21));
            signed_index_t v_neigh_opposite = tet_vertex(next_t,f12);
            t2ft1 = find_tet_vertex(t2, v_neigh_opposite);
            t2fborder = cur_f;
            
            //  Test whether the found neighboring tet was created
            //  (then return true) or is an old tet in conflict
            //  (then return false).
            return(t2 != cur_t);
        }

        StellateConflictStack S2_;
        
        

        void show_tet_adjacent(index_t t, index_t lf) const {
            signed_index_t adj = tet_adjacent(t, lf);
            if(adj != -1) {
                std::cerr << (tet_is_in_list(index_t(adj)) ? '*' : ' ');
            }
            std::cerr << adj;
            std::cerr << ' ';
        }

        
        void show_tet(index_t t) const {
            std::cerr << "tet"
                      << (tet_is_in_list(t) ? '*' : ' ')
                      << t
                      << ", v=["
                      << tet_vertex(t, 0)
                      << ' '
                      << tet_vertex(t, 1)
                      << ' '
                      << tet_vertex(t, 2)
                      << ' '
                      << tet_vertex(t, 3)
                      << "]  adj=[";
            show_tet_adjacent(t, 0);
            show_tet_adjacent(t, 1);
            show_tet_adjacent(t, 2);
            show_tet_adjacent(t, 3);
            std::cerr << "] ";
            
            for(index_t f = 0; f < 4; ++f) {
                std::cerr << 'f' << f << ':';
                for(index_t v = 0; v < 3; ++v) {
                    std::cerr << tet_vertex(t, tet_facet_vertex(f,v))
                              << ',';
                }
                std::cerr << ' ';
            }
            std::cerr << std::endl;
        }

    public:

        void check_combinatorics(bool verbose) const {
            if(verbose) {
                std::cerr << std::endl;
            }
            bool ok = true;
            std::vector<bool> v_has_tet(nb_vertices(), false);
            for(index_t t = 0; t < max_t(); ++t) {
                if(tet_is_free(t)) {
                    if(verbose) {
                        std::cerr << "-Deleted tet: ";
                        show_tet(t);
                    }
                } else {
                    if(verbose) {
                        std::cerr << "Checking tet: ";
                        show_tet(t);
                    }
                    for(index_t lf = 0; lf < 4; ++lf) {
                        if(tet_adjacent(t, lf) == -1) {
                            std::cerr << lf << ":Missing adjacent tet"
                                      << std::endl;
                            ok = false;
                        } else if(tet_adjacent(t, lf) == signed_index_t(t)) {
                            std::cerr << lf << ":Tet is adjacent to itself"
                                      << std::endl;
                            ok = false;
                        } else {
                            index_t t2 = index_t(tet_adjacent(t, lf));
                            bool found = false;
                            for(index_t lf2 = 0; lf2 < 4; ++lf2) {
                                if(tet_adjacent(t2, lf2) == signed_index_t(t)) {
                                    found = true;
                                }
                            }
                            if(!found) {
                                std::cerr
                                    << lf 
                                    << ":Adjacent link is not bidirectional"
                                    << std::endl;
                                ok = false;
                            }
                        }
                    }
                    index_t nb_infinite = 0;
                    for(index_t lv = 0; lv < 4; ++lv) {
                        if(tet_vertex(t, lv) == -1) {
                            ++nb_infinite;
                        }
                    }
                    if(nb_infinite > 1) {
                        ok = false;
                        std::cerr << "More than one infinite vertex"
                                  << std::endl;
                    }
                }
                for(index_t lv = 0; lv < 4; ++lv) {
                    signed_index_t v = tet_vertex(t, lv);
                    if(v >= 0) {
                        v_has_tet[index_t(v)] = true;
                    }
                }
            }
            for(index_t v = 0; v < nb_vertices(); ++v) {
                if(!v_has_tet[v]) {
                    if(verbose) {
                        std::cerr << "Vertex " << v
                                  << " is isolated (duplicated ?)" << std::endl;
                    }
                }
            }
            geo_assert(ok);
            if(verbose) {
                std::cerr << std::endl;
            }
            std::cerr << std::endl << "Delaunay Combi OK" << std::endl;
        }


        void check_geometry(bool verbose) const {
            bool ok = true;
            for(index_t t = 0; t < max_t(); ++t) {
                if(!tet_is_free(t)) {
                    signed_index_t v0 = tet_vertex(t, 0);
                    signed_index_t v1 = tet_vertex(t, 1);
                    signed_index_t v2 = tet_vertex(t, 2);
                    signed_index_t v3 = tet_vertex(t, 3);
                    for(index_t v = 0; v < nb_vertices(); ++v) {
                        signed_index_t sv = signed_index_t(v);
                        if(sv == v0 || sv == v1 || sv == v2 || sv == v3) {
                            continue;
                        }
                        if(tet_is_in_conflict(t, vertex_ptr(v))) {
                            ok = false;
                            if(verbose) {
                                std::cerr << "Tet " << t <<
                                    " is in conflict with vertex " << v
                                          << std::endl;
                                
                                std::cerr << "  offending tet: ";
                                show_tet(t);
                            }
                        }
                    }
                }
            }
            geo_assert(ok);
            std::cerr << std::endl << "Delaunay Geo OK" << std::endl;
        }

    private:
        ParallelDelaunay3d* master_;
        index_t nb_vertices_;
        const double* vertices_;
        const double* heights_;
        index_t* reorder_;
        index_t dimension_;
        index_t vertex_stride_;
        bool weighted_;
        index_t max_t_;
        index_t max_used_t_;

        vector<signed_index_t>& cell_to_v_store_;
        vector<signed_index_t>& cell_to_cell_store_;
        vector<index_t>& cell_next_;
        vector<thread_index_t>& cell_thread_;
        
        index_t first_free_;
        index_t nb_free_;
        bool memory_overflow_;

        index_t v1_,v2_,v3_,v4_; // The first four vertices

        vector<index_t> S_;
        index_t nb_tets_to_create_;
        index_t t_boundary_; // index of a tet,facet on the bndry 
        index_t f_boundary_; // of the conflict zone.

        bool direction_;
        signed_index_t work_begin_;
        signed_index_t work_end_;
        index_t b_hint_;
        index_t e_hint_;
        bool finished_;

        //  Whenever acquire_tet() is unsuccessful, contains
        // the index of the thread that was interfering
        // (shifted to the left by 1 !!)
        thread_index_t interfering_thread_;

#ifdef GEO_DEBUG
        index_t nb_acquired_tets_;
#endif

        vector<index_t> tets_to_delete_;
        vector<index_t> tets_to_release_;

        index_t nb_rollbacks_;
        index_t nb_failed_locate_;

        pthread_cond_t cond_;
        pthread_mutex_t mutex_;
        
        static char tet_facet_vertex_[4][3];

        static char halfedge_facet_[4][4];

	Cavity cavity_;
    };


    char Delaunay3dThread::halfedge_facet_[4][4] = {
        {4, 2, 3, 1},
        {3, 4, 0, 2},
        {1, 3, 4, 0},
        {2, 0, 1, 4}
    };

    // tet facet vertex is such that the tetrahedron
    // formed with:
    //  vertex lv
    //  tet_facet_vertex[lv][0]
    //  tet_facet_vertex[lv][1]
    //  tet_facet_vertex[lv][2]
    // has the same orientation as the original tetrahedron for
    // any vertex lv.

    char Delaunay3dThread::tet_facet_vertex_[4][3] = {
        {1, 2, 3},
        {0, 3, 2},
        {3, 0, 1},
        {1, 0, 2}
    };


    

    ParallelDelaunay3d::ParallelDelaunay3d(
        coord_index_t dimension
    ) : Delaunay(dimension) {
        if(dimension != 3 && dimension != 4) {
            throw InvalidDimension(dimension, "Delaunay3d", "3 or 4");
        }

	geo_cite_with_info(
	    "DBLP:journals/cj/Bowyer81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
        );
	geo_cite_with_info(
	    "journals/cj/Watson81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
	);
	geo_cite_with_info(
	    "DBLP:conf/compgeom/AmentaCR03",
	    "Using spatial sorting has a dramatic impact on the performances."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/FunkeMN05",
	    "Initializing \\verb|locate()| with a non-exact version "
	    " (structural filtering) gains (a bit of) performance."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/BoissonnatDPTY02",
	    "The idea of traversing the cavity from inside "
	    " used in GEOGRAM is inspired by the implementation of "
	    " \\verb|Delaunay_triangulation_3| in CGAL."
	);
	geo_cite_with_info(
	    "DBLP:conf/imr/Si06",
	    "The triangulation data structure used in GEOGRAM is inspired "
	    "by Tetgen."
	);
	geo_cite_with_info(
	    "DBLP:journals/ijfcs/DevillersPT02",
	    "Analysis of the different versions of the line walk algorithm "
	    " used by \\verb|locate()|."
	);
	
        weighted_ = (dimension == 4);
        // In weighted mode, vertices are 4d but combinatorics is 3d.
        if(weighted_) {
            cell_size_ = 4;
            cell_v_stride_ = 4;
            cell_neigh_stride_ = 4;
        }
        debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay");
        verbose_debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay_verbose");
        debug_mode_ = (debug_mode_ || verbose_debug_mode_);
        benchmark_mode_ = CmdLine::get_arg_bool("dbg:delaunay_benchmark");
    }

    void ParallelDelaunay3d::set_vertices(
        index_t nb_vertices, const double* vertices
    ) {
        Stopwatch* W = nullptr ;
        if(benchmark_mode_) {
            W = new Stopwatch("DelInternal");
        }

        if(weighted_) {
            heights_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                // Client code uses 4d embedding with ti = sqrt(W - wi)
                //   where W = max(wi)
                // We recompute the standard "shifted" lifting on
                // the paraboloid from it.
                // (we use wi - W, everything is shifted by W, but
                // we do not care since the power diagram is invariant
                // by a translation of all weights).
                double w = -geo_sqr(vertices[4 * i + 3]);
                heights_[i] = -w +
                    geo_sqr(vertices[4 * i]) +
                    geo_sqr(vertices[4 * i + 1]) +
                    geo_sqr(vertices[4 * i + 2]);
            }
        }
        Delaunay::set_vertices(nb_vertices, vertices);

        index_t expected_tetra = nb_vertices * 7;
    
        // Allocate the tetrahedra
        cell_to_v_store_.assign(expected_tetra * 4,-1);
        cell_to_cell_store_.assign(expected_tetra * 4,-1);
        cell_next_.assign(expected_tetra,index_t(-1));
        cell_thread_.assign(expected_tetra,thread_index_t(-1));

        // Reorder the points
        if(do_reorder_) {
            compute_BRIO_order(
                nb_vertices, vertex_ptr(0), reorder_,
		3, dimension(),
                64, 0.125,
                &levels_
            );        
        } else {
            reorder_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                reorder_[i] = i;
            }
            geo_debug_assert(levels_[0] == 0);
            geo_debug_assert(levels_[levels_.size()-1] == nb_vertices);
        }

        double sorting_time = 0;
        if(benchmark_mode_) {
            sorting_time = W->elapsed_time();
            Logger::out("DelInternal1") << "BRIO sorting:"
                                       << sorting_time
                                       << std::endl;
        } 

        // Create the threads
        index_t nb_threads = Process::maximum_concurrent_threads();
        index_t pool_size = expected_tetra / nb_threads;
        index_t pool_begin = 0;
        threads_.clear();
        for(index_t t=0; t<nb_threads; ++t) {
            index_t pool_end = 
                (t == nb_threads - 1) ? expected_tetra : pool_begin + pool_size;
            threads_.push_back(
                new Delaunay3dThread(this, pool_begin, pool_end)
            );
            pool_begin = pool_end;
        }


        // Create first tetrahedron and triangulate first set of points 
        // in sequential mode.


        index_t lvl = 1;
        while(lvl < (levels_.size() - 1) && levels_[lvl] < 1000) {
            ++lvl;
        }

        if(benchmark_mode_) {
            Logger::out("PDEL")
                << "Using " << levels_.size()-1 << " levels" << std::endl;
            Logger::out("PDEL") 
                << "Levels 0 - " << lvl-1 
                << ": bootstraping with first levels in sequential mode"
                << std::endl;
        }
        Delaunay3dThread* thread0 = 
                    static_cast<Delaunay3dThread*>(threads_[0].get());
        thread0->create_first_tetrahedron();
        thread0->set_work(levels_[0], levels_[lvl]);
        thread0->run();

        index_t first_lvl = lvl;

        // Insert points in all BRIO levels
        for(; lvl<levels_.size()-1; ++lvl) {

            if(benchmark_mode_) {
                Logger::out("PDEL") << "Level " 
                                    << lvl << " : start" << std::endl;
            }

            index_t lvl_b = levels_[lvl];
            index_t lvl_e = levels_[lvl+1];
            index_t work_size = (lvl_e - lvl_b)/index_t(threads_.size());

            // Initialize threads
            index_t b = lvl_b;
            for(index_t t=0; t<threads_.size(); ++t) {
                index_t e = t == threads_.size()-1 ? lvl_e : b+work_size;
                Delaunay3dThread* thread = 
                    static_cast<Delaunay3dThread*>(threads_[t].get());
                
                // Copy the indices of the first created tetrahedron
                // and the maximum valid tetrahedron index max_t_
                if(lvl == first_lvl && t!=0) {
                    thread->initialize_from(thread0);
                }
                thread->set_work(b,e);
                b = e;
            }
            Process::run_threads(threads_);
        }


        if(benchmark_mode_) {
            index_t tot_rollbacks = 0 ;
            index_t tot_failed_locate = 0 ;
            for(index_t t=0; t<threads_.size(); ++t) {
                Delaunay3dThread* thread = 
                    static_cast<Delaunay3dThread*>(threads_[t].get());
                Logger::out("PDEL") 
                    << "thread " << t << " : " 
                    << thread->nb_rollbacks() << " rollbacks  "
                    << thread->nb_failed_locate() << " failed locate"
                    << std::endl;
                tot_rollbacks += thread->nb_rollbacks();
                tot_failed_locate += thread->nb_failed_locate();
            }
            Logger::out("PDEL") << "------------------" << std::endl;
            Logger::out("PDEL") << "total: " 
                                << tot_rollbacks << " rollbacks  "
                                << tot_failed_locate << " failed locate"
                                << std::endl;
        }

        // Run threads sequentialy, to insert missing points if
        // memory overflow was encountered (in sequential mode,
        // dynamic memory growing works)

        index_t nb_sequential_points = 0;
        for(index_t t=0; t<threads_.size(); ++t) {
            Delaunay3dThread* t1 = 
                static_cast<Delaunay3dThread*>(threads_[t].get());

            nb_sequential_points += t1->work_size();

            if(t != 0) {
                // We need to copy max_t_ from previous thread, 
                // since the memory pool may have grown.
                Delaunay3dThread* t2 = 
                    static_cast<Delaunay3dThread*>(threads_[t-1].get());
                t1->initialize_from(t2);
            }
            t1->run();
        }

        //  If some tetrahedra were created in sequential mode, then
        // the maximum valid tetrahedron index was increased by all
        // the threads in increasing number, so we copy it from the
        // last thread into thread0 since we use thread0 afterwards
        // to do the "compaction" afterwards.
        
        if(nb_sequential_points != 0) {
            Delaunay3dThread* t0 = 
                static_cast<Delaunay3dThread*>(threads_[0].get());
            Delaunay3dThread* tn = 
                static_cast<Delaunay3dThread*>(
                    threads_[threads_.size()-1].get()
                );
            t0->initialize_from(tn);
        }
        

        
        if(benchmark_mode_) {
            if(nb_sequential_points != 0) {
                Logger::out("PDEL") << "Local thread memory overflow occurred:"
                                    << std::endl;
                Logger::out("PDEL") << nb_sequential_points
                                    << " points inserted in sequential mode"
                                    << std::endl;
            } else {
                Logger::out("PDEL") 
                    << "All the points were inserted in parallel mode"
                    << std::endl;
            }
        }

        if(benchmark_mode_) {
            Logger::out("DelInternal2") << "Core insertion algo:"
                                       << W->elapsed_time() - sorting_time
                                       << std::endl;
        }
        delete W;

        if(debug_mode_) {
            for(index_t i=0; i<threads_.size(); ++i) {
                std::cerr << i << " : " <<
                    static_cast<Delaunay3dThread*>(threads_[i].get())
                    ->max_t() << std::endl;
            }
            
            thread0->check_combinatorics(verbose_debug_mode_);
            thread0->check_geometry(verbose_debug_mode_);
        }

        if(benchmark_mode_) {
            W = new Stopwatch("DelCompress");
        }

        //   Compress cell_to_v_store_ and cell_to_cell_store_
        // (remove free and virtual tetrahedra).
        //   Since cell_next_ is not used at this point,
        // we reuse it for storing the conversion array that
        // maps old tet indices to new tet indices
        // Note: tet_is_real() uses the previous value of 
        // cell_next(), but we are processing indices
        // in increasing order and since old2new[t] is always
        // smaller or equal to t, we never overwrite a value
        // before needing it.
        
        vector<index_t>& old2new = cell_next_;
        index_t nb_tets = 0;
        index_t nb_tets_to_delete = 0;

        {
            for(index_t t = 0; t < thread0->max_t(); ++t) {
                if(
                    (keep_infinite_ && !thread0->tet_is_free(t)) ||
                    thread0->tet_is_real(t)
                ) {
                    if(t != nb_tets) {
                        Memory::copy(
                            &cell_to_v_store_[nb_tets * 4],
                            &cell_to_v_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                        Memory::copy(
                            &cell_to_cell_store_[nb_tets * 4],
                            &cell_to_cell_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                    }
                    old2new[t] = nb_tets;
                    ++nb_tets;
                } else {
                    old2new[t] = index_t(-1);
                    ++nb_tets_to_delete;
                }
            }

            cell_to_v_store_.resize(4 * nb_tets);
            cell_to_cell_store_.resize(4 * nb_tets);
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                // Note: t can be equal to -1 when a real tet is
                // adjacent to a virtual one (and this is how the
                // rest of Vorpaline expects to see tets on the
                // border).
                geo_debug_assert(!(keep_infinite_ && t < 0));
                cell_to_cell_store_[i] = t;
            }
        }

        // In "keep_infinite" mode, we reorder the cells in such
        // a way that finite cells have indices [0..nb_finite_cells_-1]
        // and infinite cells have indices [nb_finite_cells_ .. nb_cells_-1]
        
        if(keep_infinite_) {
            nb_finite_cells_ = 0;
            index_t finite_ptr = 0;
            index_t infinite_ptr = nb_tets - 1;
            for(;;) {
                while(thread0->tet_is_finite(finite_ptr)) {
                    old2new[finite_ptr] = finite_ptr;
                    ++finite_ptr;
                    ++nb_finite_cells_;
                }
                while(!thread0->tet_is_finite(infinite_ptr)) {
                    old2new[infinite_ptr] = infinite_ptr;
                    --infinite_ptr;
                }
                if(finite_ptr > infinite_ptr) {
                    break;
                }
                old2new[finite_ptr] = infinite_ptr;
                old2new[infinite_ptr] = finite_ptr;
                ++nb_finite_cells_;
                for(index_t lf=0; lf<4; ++lf) {
                    std::swap(
                        cell_to_cell_store_[4*finite_ptr + lf],
                        cell_to_cell_store_[4*infinite_ptr + lf]
                    );
                }
                for(index_t lv=0; lv<4; ++lv) {
                    std::swap(
                        cell_to_v_store_[4*finite_ptr + lv],
                        cell_to_v_store_[4*infinite_ptr + lv]
                    );
                }
                ++finite_ptr;
                --infinite_ptr;
            }
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                geo_debug_assert(t >= 0);
                cell_to_cell_store_[i] = t;
            }
        }
        
        
        if(benchmark_mode_) {
            if(keep_infinite_) {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list)" << std::endl;
            } else {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list and infinite)" << std::endl;
            }
        }

        delete W;

        set_arrays(
            nb_tets,
            cell_to_v_store_.data(),
            cell_to_cell_store_.data()
        );
    }
    
    index_t ParallelDelaunay3d::nearest_vertex(const double* p) const {
        // TODO
        return Delaunay::nearest_vertex(p);
    }

    void ParallelDelaunay3d::set_BRIO_levels(const vector<index_t>& levels) {
        levels_ = levels;
    }
    
}

#endif

/******* extracted from ../voronoi/convex_cell.cpp *******/


#ifndef STANDALONE_CONVEX_CELL
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

namespace VBW {
    
    ConvexCell::ConvexCell(ConvexCellFlags flags) :
	max_t_(64),
	max_v_(32),
	t_(max_t_),
	vv2t_(max_v_*max_v_),
	plane_eqn_(max_v_)
    {
#ifndef STANDALONE_CONVEX_CELL
	use_exact_predicates_ = true;
#endif	
	nb_t_ = 0;
	nb_v_ = 0;
	first_free_ = END_OF_LIST;
        first_valid_ = END_OF_LIST;
	geometry_dirty_ = true;
	has_vglobal_ = ((flags & WithVGlobal) != 0);
	if(has_vglobal_) {
	    vglobal_.assign(max_v_,index_t(-1));
	}
	has_tflags_  = ((flags & WithTFlags)  != 0);
	if(has_tflags_) {
	    tflags_.assign(max_t_,0);
	}
    }

    

    void ConvexCell::clear() {
	nb_t_ = 0;
	nb_v_ = 0;
	first_free_ = END_OF_LIST;
        first_valid_ = END_OF_LIST;
	geometry_dirty_ = true;
#ifdef VBW_DEBUG
	// Initialize all triangle flags with something
	// different from VALID_TRIANGLE.
	for(index_t t=0; t<max_t(); ++t) {
	    set_triangle_flags(t, END_OF_LIST);
	}

	// Initializing edge table, just for
	// debugging purposes.
	/*
	for(index_t v1=0; v1<max_v(); ++v1) {
	    for(index_t v2=0; v2<max_v(); ++v2) {
		set_vv2t(v1, v2, END_OF_LIST);
	    }
	}*/
#endif
    }
    
    

    void ConvexCell::init_with_box(
	double xmin, double ymin, double zmin,
	double xmax, double ymax, double zmax
    ) {
	clear();

	// The vertex at infinity.
 	plane_eqn_[0] = make_vec4(0,0,0,0);

	// Offset for the 6 bounding box plane equations.
	// Here they come first (offset is zero).
	index_t boff = 1;
	
	// The equations of the six faces of the bounding box.
	plane_eqn_[boff  ] = make_vec4( 1.0, 0.0, 0.0, -xmin);
	plane_eqn_[boff+1] = make_vec4(-1.0, 0.0, 0.0,  xmax);	
	plane_eqn_[boff+2] = make_vec4( 0.0, 1.0, 0.0, -ymin);
	plane_eqn_[boff+3] = make_vec4( 0.0,-1.0, 0.0,  ymax);	
	plane_eqn_[boff+4] = make_vec4( 0.0, 0.0, 1.0, -zmin);
	plane_eqn_[boff+5] = make_vec4( 0.0, 0.0,-1.0,  zmax);	

        //   Create the 8 triangles that correspond to the
        // 8 vertices of the bounding box.
 	//   (Unused) adjacency info. ----------------.
	//   Triangle vertices -.                     |
        //                      v                     v
	new_triangle(         boff+2,boff+5,boff+0, 1,4,2);
	new_triangle(         boff+5,boff+3,boff+0, 5,0,3);
	new_triangle(         boff+1,boff+5,boff+2, 0,6,3);
	new_triangle(         boff+5,boff+1,boff+3, 7,1,2);
	new_triangle(         boff+4,boff+2,boff+0, 0,5,6);
	new_triangle(         boff+4,boff+0,boff+3, 1,7,4);
	new_triangle(         boff+2,boff+4,boff+1, 7,2,4);
	new_triangle(         boff+4,boff+3,boff+1, 3,6,5);

	// We already created 6 vertices (for the 6 bounding box
	// plane equations) plus the vertex at infinity.
	nb_v_ = 7;

	geometry_dirty_ = true;
    }
    
    
    

    void ConvexCell::save(const std::string& filename, double shrink) const {
	std::cerr << "====> Saving " << filename << std::endl;	
	std::ofstream out(filename.c_str());
	save(out, 1, shrink);
    }
    
    
    index_t ConvexCell::save(
	std::ostream& out, index_t v_offset, double shrink, bool borders_only
    ) const {

	vec3 g = make_vec3(0.0, 0.0, 0.0);
	if(shrink != 0.0) {
	    const_cast<ConvexCell*>(this)->compute_geometry();
	    g = barycenter();
	}
	
	vector<index_t> v2t(nb_v(),index_t(-1));
	vector<index_t> t_index(nb_t(),index_t(-1));
	index_t nt=0;

	{
	    index_t t = first_valid_;
	    while(t != END_OF_LIST) { 
		TriangleWithFlags T = get_triangle_and_flags(t);
		vec4 p = compute_triangle_point(T);
		p.x /= p.w;
		p.y /= p.w;
		p.z /= p.w;
		p.w = 1.0;
		if(shrink != 0.0) {
		    p.x = shrink * g.x + (1.0 - shrink) * p.x;
		    p.y = shrink * g.y + (1.0 - shrink) * p.y;
		    p.z = shrink * g.z + (1.0 - shrink) * p.z;
		}
		out << "v " << p.x << " " << p.y << " " << p.z << std::endl;
		t_index[t] = nt;
		++nt;
		v2t[T.i] = t;
		v2t[T.j] = t;
		v2t[T.k] = t;
		t = index_t(T.flags);
	    }
	}
	
	for(index_t v=1; v<nb_v(); ++v) {
	    if(borders_only &&
	       has_vglobal() &&
	       v_global_index(v) != global_index_t(-1) &&
	       v_global_index(v) != global_index_t(-2) // This one for fluid free bndry
	    ) {
		continue;
	    }
	    if(v2t[v] != index_t(-1)) {
		index_t t = v2t[v];
		out << "f ";
		do {
		    out << (t_index[t]+v_offset) << " ";
		    index_t lv = triangle_find_vertex(t,v);		   
		    t = triangle_adjacent(t, (lv + 1)%3);
		} while(t != v2t[v]);
		out << std::endl;
	    }
	}

	return nt;
    }

#if !defined(STANDALONE_CONVEX_CELL) && !defined(GEOGRAM_PSM)
    
    void ConvexCell::append_to_mesh(
	GEO::Mesh* mesh, double shrink, bool borders_only,
	GEO::Attribute<GEO::index_t>* facet_attr
    ) const {

	index_t v_offset = mesh->vertices.nb();
	
	vec3 g = make_vec3(0.0, 0.0, 0.0);
	if(shrink != 0.0) {
	    const_cast<ConvexCell*>(this)->compute_geometry();
	    g = barycenter();
	}
	
	vector<index_t> v2t(nb_v(),index_t(-1));
	vector<index_t> t_index(nb_t(),index_t(-1));
	index_t nt=0;

	{
	    index_t t = first_valid_;
	    while(t != END_OF_LIST) { 
		TriangleWithFlags T = get_triangle_and_flags(t);
		vec4 p = compute_triangle_point(T);
		p.x /= p.w;
		p.y /= p.w;
		p.z /= p.w;
		p.w = 1.0;
		if(shrink != 0.0) {
		    p.x = shrink * g.x + (1.0 - shrink) * p.x;
		    p.y = shrink * g.y + (1.0 - shrink) * p.y;
		    p.z = shrink * g.z + (1.0 - shrink) * p.z;
		}
		mesh->vertices.create_vertex(p.data());
		t_index[t] = nt;
		++nt;
		v2t[T.i] = t;
		v2t[T.j] = t;
		v2t[T.k] = t;
		t = index_t(T.flags);
	    }
	}
	
	for(index_t v=1; v<nb_v(); ++v) {
	    if(borders_only &&
	       has_vglobal() &&
	       v_global_index(v) != global_index_t(-1) &&
	       v_global_index(v) != global_index_t(-2) // This one for fluid free bndry
	    ) {
		continue;
	    }
	    std::vector<index_t> facet_vertices;
	    if(v2t[v] != index_t(-1)) {
		index_t t = v2t[v];
		do {
		    facet_vertices.push_back(t_index[t]+v_offset);
		    index_t lv = triangle_find_vertex(t,v);		   
		    t = triangle_adjacent(t, (lv + 1)%3);
		} while(t != v2t[v]);
	    }
	    if(facet_vertices.size() < 3) {
		continue;
	    }
	    index_t f = mesh->facets.create_polygon(GEO::index_t(facet_vertices.size()));
	    for(index_t i=0; i<facet_vertices.size(); ++i) {
		mesh->facets.set_vertex(f, i, facet_vertices[i]);
	    }
	    if(facet_attr != nullptr && facet_attr->is_bound()) {
		(*facet_attr)[f] = v_global_index(v);
	    }
	}
	
    }

#endif
    
    

    bool ConvexCell::has_v_global_index(global_index_t v) const {
	vbw_assert(has_vglobal_);
	for(index_t i=0; i<nb_v(); ++i) {
	    if(vglobal_[i] == v) {
		return true;
	    }
	}
	return false;
    }
    
    void ConvexCell::clip_by_plane(vec4 eqn, global_index_t j) {
	vbw_assert(has_vglobal_);
	clip_by_plane(eqn);
	vglobal_[nb_v()-1] = j;
    }
	
    void ConvexCell::clip_by_plane(vec4 eqn) {
	geometry_dirty_ = true;

	index_t lv = nb_v_;	
	if(lv == max_v()) {
	    grow_v();
	}
	plane_eqn_[lv] = eqn;
	vbw_assert(lv < max_v());
	++nb_v_;
	
	// Step 1: Find conflict zone and link conflicted triangles
	// (to recycle them in free list).

	index_t conflict_head = END_OF_LIST;
	index_t conflict_tail = END_OF_LIST;

        // Classify triangles, compute conflict list and valid list.
	// Note: This could be done by climbing from a random triangle,
	// but here we prefer complete linear scan for several reasons:
	//   - it is more robust to numerical errors (here we are not
	//     using exact predicates).
	//   - the code is simpler.
	//   - and more importantly, we got no more than a few tenths of
	//     vertices.

        index_t t = first_valid_;
        first_valid_ = END_OF_LIST;
        while(t != END_OF_LIST) { 
	    TriangleWithFlags T = get_triangle_and_flags(t);
	    if(triangle_is_in_conflict(T,eqn)) {
		set_triangle_flags(
		    t, ushort(conflict_head) | ushort(CONFLICT_MASK)
		);
		conflict_head = t;
		if(conflict_tail == END_OF_LIST) {
		    conflict_tail = t;
		}
	    } else {
		set_triangle_flags(t, ushort(first_valid_));
		first_valid_ = t;
	    }
	    t = index_t(T.flags);
	} 

	// Special case: no triangle in conflict.
	if(conflict_head == END_OF_LIST) {
	    return;
	}

	// Step 2: Triangulate cavity

        t = conflict_head;
        while(t != END_OF_LIST) { 
	    TriangleWithFlags T = get_triangle_and_flags(t);
	    
	    index_t adj1 = vv2t(T.j,T.i);
	    index_t adj2 = vv2t(T.k,T.j);
	    index_t adj3 = vv2t(T.i,T.k);	       
	    
	    ushort flg1 = get_triangle_flags(adj1);
	    ushort flg2 = get_triangle_flags(adj2);
	    ushort flg3 = get_triangle_flags(adj3);		
	    
	    if(	!(flg1 & ushort(CONFLICT_MASK)) ) {
		new_triangle(lv, T.i, T.j);
	    }

	    if(	!(flg2 & ushort(CONFLICT_MASK)) ) {
		new_triangle(lv, T.j, T.k);		
	    }

	    if(	!(flg3 & ushort(CONFLICT_MASK)) ) {
		new_triangle(lv, T.k, T.i);
	    }
	       
	    t = index_t(T.flags & ~ushort(CONFLICT_MASK));
	} 
	
	// Step 3: Recycle conflict list.
	
	set_triangle_flags(conflict_tail, ushort(first_free_));
	first_free_ = conflict_head;

    }

    

    bool ConvexCell::triangle_is_in_conflict(
	TriangleWithFlags T, const vec4& eqn
    ) const {
#ifndef STANDALONE_CONVEX_CELL
	if(use_exact_predicates_) {
	    if(T.i == VERTEX_AT_INFINITY) {
		vec3 E  = make_vec3(eqn.x, eqn.y, eqn.z);
		vec3 n2 = vertex_plane_normal(T.j);
		vec3 n3 = vertex_plane_normal(T.k);
		return(GEO::PCK::det_3d(E.data(), n2.data(), n3.data()) <= 0);
	    }

	    if(T.j == VERTEX_AT_INFINITY) {
		vec3 E  = make_vec3(eqn.x, eqn.y, eqn.z);	    
		vec3 n3 = vertex_plane_normal(T.k);
		vec3 n1 = vertex_plane_normal(T.i);
		return(GEO::PCK::det_3d(n1.data(), E.data(), n3.data()) <= 0);
	    }

	    if(T.k == VERTEX_AT_INFINITY) {
		vec3 E  = make_vec3(eqn.x, eqn.y, eqn.z);	    	    
		vec3 n1 = vertex_plane_normal(T.i);
		vec3 n2 = vertex_plane_normal(T.j);
		return(GEO::PCK::det_3d(n1.data(), n2.data(), E.data()) <= 0);
	    } 
	    
	    //   The triangle is in conflict with eqn if the 
	    // result of compute_triangle_point(t) injected in eqn 
	    // has a negative sign.
	    //   Examining the formula in compute_triangle_point(), this corresponds
	    // to (minus) the 4x4 determinant of the 4 plane equations
	    // developed w.r.t. the 4th column.
	    // (see Edelsbrunner - Simulation of Simplicity for similar examples
	    //  of computations).
	
	    vec4 p1 = vertex_plane(T.i);
	    vec4 p2 = vertex_plane(T.j);
	    vec4 p3 = vertex_plane(T.k);
	    
	    return(GEO::PCK::det_4d(p1.data(), p2.data(), p3.data(), eqn.data()) >= 0);
	}
#endif
	
	double det = 0.0;

	// If one of the vertices of the triangle is the
	// vertex at infinity, then the triangle is in conflict
	// with eqn if the oriented director vector of the intersection
	// between the two planes of the two other vertices
	// is opposite to the normal vector to eqn.

	if(T.i == VERTEX_AT_INFINITY) {
	    vec3 n2 = vertex_plane_normal(T.j);
	    vec3 n3 = vertex_plane_normal(T.k);
	    det = -det3x3(
		eqn.x, n2.x, n3.x, 
		eqn.y, n2.y, n3.y, 
		eqn.z, n2.z, n3.z
	    );
	} else if(T.j == VERTEX_AT_INFINITY) {
	    vec3 n3 = vertex_plane_normal(T.k);
	    vec3 n1 = vertex_plane_normal(T.i);
	    det = -det3x3(
		n1.x, eqn.x, n3.x,
		n1.y, eqn.y, n3.y,
		n1.z, eqn.z, n3.z
	    );
	} else if(T.k == VERTEX_AT_INFINITY) {
	    vec3 n1 = vertex_plane_normal(T.i);
	    vec3 n2 = vertex_plane_normal(T.j);
	    det = -det3x3(
		n1.x, n2.x, eqn.x,
		n1.y, n2.y, eqn.y,
		n1.z, n2.z, eqn.z
	    );
	} else {
	    
	    //   The triangle is in conflict with eqn if the 
	    // result of compute_triangle_point(t) injected in eqn 
	    // has a negative sign.
	    //   Examining the formula in compute_triangle_point(), this corresponds
	    // to (minus) the 4x4 determinant of the 4 plane equations
	    // developed w.r.t. the 4th column.
	    // (see Edelsbrunner - Simulation of Simplicity for similar examples
	    //  of computations).
	
	    vec4 p1 = vertex_plane(T.i);
	    vec4 p2 = vertex_plane(T.j);
	    vec4 p3 = vertex_plane(T.k);
	    det = det4x4(
		p1.x, p2.x, p3.x, eqn.x,
		p1.y, p2.y, p3.y, eqn.y,
		p1.z, p2.z, p3.z, eqn.z,
		p1.w, p2.w, p3.w, eqn.w
	    );
	}
	return (det > 0.0);
    }
    
    vec4 ConvexCell::compute_triangle_point(TriangleWithFlags T) const {

	double infinite_len = 16.0;
	
	// Special cases with one of the three vertices at infinity.
	if(T.i == VERTEX_AT_INFINITY) {
	    vec4 Pj = vertex_plane(T.j);
	    vec4 Pk = vertex_plane(T.k);
	    vec3 Njk = normalize(cross(
		make_vec3(Pj.x, Pj.y, Pj.z),
		make_vec3(Pk.x, Pk.y, Pk.z)
	    ));
	    index_t t_adj = vv2t(T.k, T.j);
	    vbw_assert(!triangle_is_infinite(t_adj));
	    TriangleWithFlags T_adj = t_[t_adj];
	    vec4 result = compute_triangle_point(T_adj);
	    result.x += result.w * Njk.x * infinite_len;
	    result.y += result.w * Njk.y * infinite_len;
	    result.z += result.w * Njk.z * infinite_len;
	    return result;
	} else if(T.j == VERTEX_AT_INFINITY) {
	    vec4 Pk = vertex_plane(T.k);
	    vec4 Pi = vertex_plane(T.i);
	    vec3 Nki = normalize(cross(
		make_vec3(Pk.x, Pk.y, Pk.z),
		make_vec3(Pi.x, Pi.y, Pi.z)
	    ));
	    index_t t_adj = vv2t(T.i, T.k);
	    vbw_assert(!triangle_is_infinite(t_adj));
	    TriangleWithFlags T_adj = t_[t_adj];
	    vec4 result = compute_triangle_point(T_adj);
	    result.x += result.w * Nki.x * infinite_len;
	    result.y += result.w * Nki.y * infinite_len;
	    result.z += result.w * Nki.z * infinite_len;
	    return result;
	} else if(T.k == VERTEX_AT_INFINITY) {
	    vec4 Pi = vertex_plane(T.i);
	    vec4 Pj = vertex_plane(T.j);
	    vec3 Nij = normalize(cross(
		make_vec3(Pi.x, Pi.y, Pi.z),
		make_vec3(Pj.x, Pj.y, Pj.z)
	    ));
	    index_t t_adj = vv2t(T.j, T.i);
	    vbw_assert(!triangle_is_infinite(t_adj));
	    TriangleWithFlags T_adj = t_[t_adj];
	    vec4 result = compute_triangle_point(T_adj);
	    result.x += result.w * Nij.x * infinite_len;
	    result.y += result.w * Nij.y * infinite_len;
	    result.z += result.w * Nij.z * infinite_len;
	    return result;
	}
	
        // Get the plane equations associated with each vertex of t
	vec4 pi1 = vertex_plane(T.i);
	vec4 pi2 = vertex_plane(T.j);
	vec4 pi3 = vertex_plane(T.k);

        // Find the intersection of the three planes using Kramer's formula.
	// (see Edelsbrunner - Simulation of Simplicity for other examples).
	// 
	// Kramer's formula: each component of the solution is obtained as
	//  the ratio of two determinants:
	//   - the determinant of the system where the ith column is replaced 
	//     with the rhs
	//   divided by:
	//   - the determinant of the system.
	// 
	// System of equations to be solved:
	// pi1.x * x + pi1.y * y + pi1.z * z = -pi1.w
	// pi2.x * x + pi2.y * y + pi2.z * z = -pi2.w
	// pi3.x * x + pi3.y * y + pi3.z * z = -pi3.w
	// 
	// Expression of the solution given by Cramer's formula:
	//     | -pi1.w   p1.y   pi1.z |   | pi1.x  pi1.y  pi1.z |
	// x = | -pi2.w   p2.y   pi2.z | / | pi2.x  pi2.y  pi2.z |
	//     | -pi3.w   p3.y   pi3.z |   | pi3.x  pi3.y  pi3.z |
       	// 
	//     |  pi1.x  -p1.w   pi1.z |   | pi1.x  pi1.y  pi1.z |
	// y = |  pi2.x  -p2.w   pi2.z | / | pi2.x  pi2.y  pi2.z |
	//     |  pi3.x  -p3.w   pi3.z |   | pi3.x  pi3.y  pi3.z |
	// 
	//     |  pi1.x   p1.y  -pi1.w |   | pi1.x  pi1.y  pi1.z |
	// z = |  pi2.x   p2.y  -pi2.w | / | pi2.x  pi2.y  pi2.z |
	//     |  pi3.x   p3.y  -pi3.w |   | pi3.x  pi3.y  pi3.z |
       
	vec4 result;

	result.x = -det3x3(
	    pi1.w, pi1.y, pi1.z,
	    pi2.w, pi2.y, pi2.z,
	    pi3.w, pi3.y, pi3.z
	);

	result.y = -det3x3(
	    pi1.x, pi1.w, pi1.z,
	    pi2.x, pi2.w, pi2.z,
	    pi3.x, pi3.w, pi3.z
	);
	
	result.z = -det3x3(
	    pi1.x, pi1.y, pi1.w,
	    pi2.x, pi2.y, pi2.w,
	    pi3.x, pi3.y, pi3.w
	);
	
	result.w = det3x3(
	    pi1.x, pi1.y, pi1.z,
	    pi2.x, pi2.y, pi2.z,
	    pi3.x, pi3.y, pi3.z
	);

	return result;
    }
    
    

    void ConvexCell::grow_v() {
	vector<ushort> vv2t_new((max_v_*2)*(max_v_*2));
	for(index_t j=0; j<max_v_; ++j) {	
	    for(index_t i=0; i<max_v_; ++i) {
		vv2t_new[max_v_ * 2 * j + i] = vv2t_[max_v_ * j + i];
	    }
	}
	std::swap(vv2t_, vv2t_new);
	max_v_ *= 2;
	plane_eqn_.resize(max_v_);
	vglobal_.resize(max_v_, global_index_t(-1));
    }

    void ConvexCell::grow_t() {
	max_t_ *= 2;
	t_.resize(max_t_);
	if(has_tflags_) {
	    tflags_.resize(max_t_,0);
	}
    }

    
    
    void ConvexCell::kill_vertex(index_t v) {
	for(index_t t=0; t<nb_t(); ++t) {
	    Triangle T = get_triangle(t);
	    if(T.i == v) {
		T.i = VERTEX_AT_INFINITY;
	    }
	    if(T.j == v) {
		T.j = VERTEX_AT_INFINITY;
	    }
	    if(T.k == v) {
		T.k = VERTEX_AT_INFINITY;
	    }
	    set_vv2t(T.i, T.j, t);
	    set_vv2t(T.j, T.k, t);
	    set_vv2t(T.k, T.i, t);
	    t_[t].i = T.i;
	    t_[t].j = T.j;
	    t_[t].k = T.k;	    
	}
    }

    
    
    void ConvexCell::compute_geometry() {
	if(!geometry_dirty_) {
	    return;
	}

	triangle_point_.resize(nb_t());
	
	// Yes, need to do that with two
	// instructions: resize(nb_v(), END_OF_LIST)
	// does not reset the existing items to
	// END_OF LIST !! (had a hard time finding
	// this bug...)
	v2t_.resize(nb_v());
	v2t_.assign(nb_v(),END_OF_LIST);

        index_t t = first_valid_;
        while(t != END_OF_LIST) { 
	    TriangleWithFlags T = get_triangle_and_flags(t);
	    vec4 p = compute_triangle_point(T);
	    triangle_point_[t] = make_vec3(p.x/p.w, p.y/p.w, p.z/p.w);
	    v2t_[T.i] = ushort(t);
	    v2t_[T.j] = ushort(t);
	    v2t_[T.k] = ushort(t);
	    t = index_t(T.flags);
	}
	
	geometry_dirty_ = false;
    }

    inline double triangle_area(vec3 p1, vec3 p2, vec3 p3) {
	double Ux = p2.x - p1.x;
	double Uy = p2.y - p1.y;
	double Uz = p2.z - p1.z;
	double Vx = p3.x - p1.x;
	double Vy = p3.y - p1.y;
	double Vz = p3.z - p1.z;
	double Wx = Uy * Vz - Uz * Vy;
	double Wy = Uz * Vx - Ux * Vz;
	double Wz = Ux * Vy - Uy * Vx;
	return 0.5 * ::sqrt(Wx*Wx + Wy*Wy + Wz*Wz);
    }
    
    double ConvexCell::facet_area(index_t v) const {
	vbw_assert(v < nb_v());
	vbw_assert(!geometry_dirty_);

	ushort t1t2[2];
	index_t cur=0;
	double result = 0.0;
	
	if(v2t_[v] != END_OF_LIST) {
	    index_t t = v2t_[v];
	    do {
		if(cur < 2) {
		    t1t2[cur] = ushort(t);
		} else {
		    result += triangle_area(
			triangle_point_[t1t2[0]],
			triangle_point_[t1t2[1]],
			triangle_point_[t]
		    );
		    t1t2[1] = ushort(t);
		}
		++cur;
		index_t lv = triangle_find_vertex(t,v);		   
		t = triangle_adjacent(t, (lv + 1)%3);
	    } while(t != v2t_[v]);
	}
	
	return result;
    }

    inline double tet_volume(vec3 p1, vec3 p2, vec3 p3, vec3 p4) {
	double Ux = p2.x - p1.x;
	double Uy = p2.y - p1.y;
	double Uz = p2.z - p1.z;
	
	double Vx = p3.x - p1.x;
	double Vy = p3.y - p1.y;
	double Vz = p3.z - p1.z;
	
	double Wx = p4.x - p1.x;
	double Wy = p4.y - p1.y;
	double Wz = p4.z - p1.z;

	double UVx = Uy * Vz - Uz * Vy;
	double UVy = Uz * Vx - Ux * Vz;
	double UVz = Ux * Vy - Uy * Vx;

	return ::fabs(
	    UVx * Wx + UVy * Wy + UVz * Wz
	) / 6.0;
    }
    
    double ConvexCell::volume() const {
	vbw_assert(!geometry_dirty_);
	double result = 0.0;

	ushort t_origin = END_OF_LIST;
	for(index_t v=0; v<nb_v_; ++v) {
	    if(v2t_[v] == END_OF_LIST) {
		continue;
	    }
	    if(t_origin == END_OF_LIST) {
		t_origin = v2t_[v];
		continue;
	    }
	    ushort t1t2[2];
	    index_t cur=0;
	    index_t t = v2t_[v];
	    do {
		if(cur < 2) {
		    t1t2[cur] = ushort(t);
		} else {
		    result += tet_volume(
			triangle_point_[t_origin],
			triangle_point_[t1t2[0]],
			triangle_point_[t1t2[1]],
			triangle_point_[t]
		    );
		    t1t2[1] = ushort(t);
		}
		++cur;
		index_t lv = triangle_find_vertex(t,v);		   
		t = triangle_adjacent(t, (lv + 1)%3);
	    } while(t != v2t_[v]);
	}
	return result;
    }

    vec3 ConvexCell::barycenter() const {
	vbw_assert(!geometry_dirty_);
	vec3 result = make_vec3(0.0, 0.0, 0.0);
	double m = 0.0;

	ushort t_origin = END_OF_LIST;
	for(index_t v=0; v<nb_v_; ++v) {
	    if(v2t_[v] == END_OF_LIST) {
		continue;
	    }
	    if(t_origin == END_OF_LIST) {
		t_origin = v2t_[v];
		continue;
	    }
	    ushort t1t2[2];
	    index_t cur=0;
	    index_t t = v2t_[v];
	    do {
		if(cur < 2) {
		    t1t2[cur] = ushort(t);
		} else {
		    vec3 p = triangle_point_[t_origin];
		    vec3 q = triangle_point_[t1t2[0]];
		    vec3 r = triangle_point_[t1t2[1]];
		    vec3 s = triangle_point_[t];
		    double cur_m = tet_volume(p,q,r,s);
		    m += cur_m;
		    result.x += cur_m*(p.x + q.x + r.x + s.x)/4.0;
		    result.y += cur_m*(p.y + q.y + r.y + s.y)/4.0;
		    result.z += cur_m*(p.z + q.z + r.z + s.z)/4.0;
		    t1t2[1] = ushort(t);
		}
		++cur;
		index_t lv = triangle_find_vertex(t,v);		   
		t = triangle_adjacent(t, (lv + 1)%3);
	    } while(t != v2t_[v]);
	}
	
	if(m != 0.0) {
	    result.x /= m;
	    result.y /= m;
	    result.z /= m;
	}
	return result;
    }
    
    

    
    double ConvexCell::squared_radius(vec3 center) const {
	double result = 0.0;
        index_t t = first_valid_;
        while(t != END_OF_LIST) { 
	    TriangleWithFlags T = get_triangle_and_flags(t);
	    if(geometry_dirty_) {
		vec4 p4 = compute_triangle_point(T);
		vec3 p3 = make_vec3(
		    p4.x/p4.w, p4.y/p4.w, p4.z/p4.w
		);
		result = std::max(result, squared_distance(center,p3));		
	    } else {
		vec3 p = triangle_point_[t];
		result = std::max(result, squared_distance(center,p));
	    }
	    t = index_t(T.flags);
	}
	return result;
    }

    double ConvexCell::squared_inner_radius(vec3 center) const {
	double result = std::numeric_limits<double>::max();
	for(index_t v=0; v<nb_v(); ++v) {
	    result = std::min(
		result, squared_point_plane_distance(center, vertex_plane(v))
	    );
	}
	return result;
    }
    
}

/******* extracted from periodic.cpp *******/


namespace GEO {

    int Periodic::translation[27][3] = {
	{  0,  0,  0}, //13 -> 0   +   <-- zero displacement is first.
	{ -1, -1, -1}, //0  -> 1   -
	{ -1, -1,  0}, //1  -> 2   -
	{ -1, -1,  1}, //2  -> 3   -
	{ -1,  0, -1}, //3  -> 4   -
	{ -1,  0,  0}, //4  -> 5   - 
	{ -1,  0,  1}, //5  -> 6   -
	{ -1,  1, -1}, //6  -> 7   -
	{ -1,  1,  0}, //7  -> 8   -
	{ -1,  1,  1}, //8  -> 9   - 
	{  0, -1, -1}, //9  -> 10  -
	{  0, -1,  0}, //10 -> 11  -
	{  0, -1,  1}, //11 -> 12  -
	{  0,  0, -1}, //12 -> 13  -
	// (zero displacement was there)
	{  0,  0,  1}, //14 -> 14  +
	{  0,  1, -1}, //15 -> 15  -
	{  0,  1,  0}, //16 -> 16  +
	{  0,  1,  1}, //17 -> 17  +
	{  1, -1, -1}, //18 -> 18  -
	{  1, -1,  0}, //19 -> 19  -
	{  1, -1,  1}, //20 -> 20  -
	{  1,  0, -1}, //21 -> 21  -
	{  1,  0,  0}, //22 -> 22  +
	{  1,  0,  1}, //23 -> 23  +
	{  1,  1, -1}, //24 -> 24  -
	{  1,  1,  0}, //25 -> 25  +
	{  1,  1,  1}  //26 -> 26  +
    };

    int Periodic::reorder_instances[27] = {
	1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
	10, 11, 12, 13, 0 , 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25, 26
    };

    bool Periodic::instance_is_positive[27] = {
	true,  false, false, false, false, false, 
	false, false, false, false, false, false, 
	false, false, true,  false, true,  true,  
	false, false, false, false, true,  true,
	false, true,  true
    };
}


/******* extracted from periodic_delaunay_3d.cpp *******/



#include <stack>
#include <algorithm>

// ParallelDelaunayThread class, declared locally, has
// no out-of-line virtual functions. It is not a
// problem since they are only visible from this translation
// unit, but clang will complain.
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wweak-vtables"
#endif

// TODO:
//  - insert additional vertices in parallel ?
//  - update v_to_cell in parallel ?


namespace {

    using namespace GEO;

    index_t thread_safe_random_(index_t choices_in) {
        signed_index_t choices = signed_index_t(choices_in);
        static thread_local long int randomseed = 1l ;
        if (choices >= 714025l) {
            long int newrandom = (randomseed * 1366l + 150889l) % 714025l;
            randomseed = (newrandom * 1366l + 150889l) % 714025l;
            newrandom = newrandom * (choices / 714025l) + randomseed;
            if (newrandom >= choices) {
                return index_t(newrandom - choices);
            } else {
                return index_t(newrandom);
            }
        } else {
            randomseed = (randomseed * 1366l + 150889l) % 714025l;
            return index_t(randomseed % choices);
        }
    }

    index_t thread_safe_random_4_() {
        static thread_local long int randomseed = 1l ;
        randomseed = (randomseed * 1366l + 150889l) % 714025l;
        return index_t(randomseed % 4);
    }

    inline index_t pop_count(index_t x) {
	static_assert(sizeof(index_t) == 4, "Only supported with 32 bit indices");
#if defined(GEO_COMPILER_GCC_FAMILY)
	return index_t(__builtin_popcount(x));
#elif defined(GEO_COMPILER_MSVC)
	return index_t(__popcnt(x));
#else
	int result = 0;
	for(int b=0; b<32; ++b) {
	    result += ((x & 1) != 0);
	    x >>= 1;
	}
	return index_t(result);
#endif	
    }
}

namespace GEO {

    class PeriodicDelaunay3dThread : public GEO::Thread, public Periodic {
    public:
	friend class PeriodicDelaunay3d;
	
        static const index_t NO_THREAD = thread_index_t(-1);

        PeriodicDelaunay3dThread(
            PeriodicDelaunay3d* master,
            index_t pool_begin,
            index_t pool_end
        ) : 
            master_(master),
	    periodic_(master->periodic_),
	    period_(master->period_),
            cell_to_v_store_(master_->cell_to_v_store_),
            cell_to_cell_store_(master_->cell_to_cell_store_),
            cell_next_(master_->cell_next_),
            cell_thread_(master_->cell_thread_),
	    has_empty_cells_(false)
        {

            max_t_ = master_->cell_next_.size();

            nb_vertices_ = master_->nb_vertices();
	    nb_vertices_non_periodic_ = master_->nb_vertices_non_periodic_;
            vertices_ = master_->vertex_ptr(0);
	    weights_ = master_->weights_;
            dimension_ = master_->dimension();
            reorder_ = master_->reorder_.data();


            nb_rollbacks_ = 0;
            nb_failed_locate_ = 0;

            v1_ = index_t(-1);
            v2_ = index_t(-1);
            v3_ = index_t(-1);
            v4_ = index_t(-1);

            pthread_cond_init(&cond_, nullptr);
            pthread_mutex_init(&mutex_, nullptr);

            b_hint_ = NO_TETRAHEDRON;
            e_hint_ = NO_TETRAHEDRON;
	    
	    set_pool(pool_begin, pool_end);
	   
        }

	void set_pool(index_t pool_begin, index_t pool_end, index_t max_used_t = 1) {
            // Initialize free list in memory pool
            first_free_ = pool_begin;
            for(index_t t=pool_begin; t<pool_end-1; ++t) {
                cell_next_[t] = t+1;
            }
            cell_next_[pool_end-1] = END_OF_LIST;
            nb_free_ = pool_end - pool_begin;
            memory_overflow_ = false;
            work_begin_ = -1;
            work_end_ = -1;
            finished_ = false;
            direction_ = true;
#ifdef GEO_DEBUG
            nb_acquired_tets_ = 0;
#endif
            interfering_thread_ = NO_THREAD;
            nb_tets_to_create_ = 0;
            t_boundary_ = NO_TETRAHEDRON;
            f_boundary_ = index_t(-1);

            // max_used_t_ is initialized to 1 so that
            // computing modulos does not trigger FPEs
            // at the beginning.
            max_used_t_ = std::max(max_used_t, index_t(1));
	}

	
        ~PeriodicDelaunay3dThread() {
            pthread_mutex_destroy(&mutex_);
            pthread_cond_destroy(&cond_);
        }

	bool has_empty_cells() {
	    return has_empty_cells_;
	}
	
        void initialize_from(const PeriodicDelaunay3dThread* rhs) {
            max_used_t_ = rhs->max_used_t_;
            max_t_ = rhs->max_t_;
            v1_ = rhs->v1_;
            v2_ = rhs->v2_;
            v3_ = rhs->v3_;
            v4_ = rhs->v4_;
        }

        index_t nb_rollbacks() const {
            return nb_rollbacks_;
        }

        index_t nb_failed_locate() const {
            return nb_failed_locate_;
        }

	index_t nb_traversed_tets() const {
	    return nb_traversed_tets_;
	}
	
        void set_work(index_t b, index_t e) {
            work_begin_ = signed_index_t(b);
            // e is one position past the last point index
            // to insert. 
            work_end_ = signed_index_t(e)-1;
        }

        index_t work_size() const {
            if(work_begin_ == -1 && work_end_ == -1) {
                return 0;
            }
            geo_debug_assert(work_begin_ != -1);
            geo_debug_assert(work_end_ != -1);
            return index_t(std::max(
			       work_end_ - work_begin_ + 1,signed_index_t(0))
	    );
        }

        index_t nb_threads() const {
            return index_t(master_->threads_.size());
        }

        PeriodicDelaunay3dThread* thread(index_t t) {
            return static_cast<PeriodicDelaunay3dThread*>(
                master_->threads_[t].get()
            );
        }

        virtual void run() {
	    has_empty_cells_ = false;
            finished_ = false;

            if(work_begin_ == -1 || work_end_ == -1) {
                return ;
            }

            memory_overflow_ = false;

            // Current hint associated with b
            b_hint_ = NO_TETRAHEDRON;

            // Current hint associated with e
            e_hint_ = NO_TETRAHEDRON;

            // If true, insert in b->e order,
            // else insert in e->b order
            direction_ = true;
            
            while(
		work_end_ >= work_begin_ &&
		!memory_overflow_ &&
		!has_empty_cells_ &&
		!master_->has_empty_cells_
	    ) {
                index_t v = direction_ ? 
                    index_t(work_begin_) : index_t(work_end_) ;
                index_t& hint = direction_ ? b_hint_ : e_hint_;

                // Try to insert v and update hint
                bool success = insert(reorder_[v],hint);

                //   Notify all threads that are waiting for
                // this thread to release some tetrahedra.
                send_event();

                if(success) {
                    if(direction_) {
                        ++work_begin_;
                    } else {
                        --work_end_;
                    }
                } else {
                    ++nb_rollbacks_;
                    if(interfering_thread_ != NO_THREAD) {
                        interfering_thread_ = thread_index_t(
                            interfering_thread_ >> 1
                        );
                        if(id() < interfering_thread_) {
                            // If this thread has a higher priority than
                            // the one that interfered, wait for the
                            // interfering thread to release the tets that
                            // it holds (then the loop will retry to insert
                            // the same vertex).
                            wait_for_event(interfering_thread_);
                        } else {
                            // If this thread has a lower priority than
                            // the interfering thread, try inserting 
                            // from the other end of the points sequence.
                            direction_ = !direction_;
                        }
                    }
                }
            }
            finished_ = true;

	    if(has_empty_cells_) {
		master_->has_empty_cells_ = true;
	    }
	    
	    //   Fix by Hiep Vu: wake up threads that potentially missed
	    // the previous wake ups.
	    pthread_mutex_lock(&mutex_);
	    send_event();
	    pthread_mutex_unlock(&mutex_);
        }

        static const index_t NO_TETRAHEDRON = index_t(-1);

        static const signed_index_t VERTEX_AT_INFINITY = -1;
        

        index_t max_t() const {
            return max_t_;
        }

        bool tet_is_finite(index_t t) const {
            return 
                cell_to_v_store_[4 * t]     >= 0 &&
                cell_to_v_store_[4 * t + 1] >= 0 &&
                cell_to_v_store_[4 * t + 2] >= 0 &&
                cell_to_v_store_[4 * t + 3] >= 0;
        }
        
        bool tet_is_real(index_t t) const {
            return !tet_is_free(t) && tet_is_finite(t);
        }

        bool tet_is_real_non_periodic(index_t t) const {
            return !tet_is_free(t) && tet_is_finite(t) && (
		finite_tet_vertex(t,0) < nb_vertices_non_periodic_ ||
		finite_tet_vertex(t,1) < nb_vertices_non_periodic_ ||
		finite_tet_vertex(t,2) < nb_vertices_non_periodic_ ||
		finite_tet_vertex(t,3) < nb_vertices_non_periodic_ ) ;					 
        }
	
        bool tet_is_free(index_t t) const {
            return tet_is_in_list(t);
        }
        
        bool tet_is_in_list(index_t t, index_t first) const {
            for(
                index_t cur = first; cur != END_OF_LIST; 
                cur = tet_next(cur)
            ) {
                if(cur == t) {
                    return true;
                }
            }
            return false;
        }


        index_t create_first_tetrahedron() {
            index_t iv0,iv1,iv2,iv3;
            if(nb_vertices() < 4) {
                return NO_TETRAHEDRON;
            }

            iv0 = 0;
            
            iv1 = 1;
            while(
                iv1 < nb_vertices_non_periodic_ &&
                PCK::points_are_identical_3d(
                    non_periodic_vertex_ptr(iv0),
		    non_periodic_vertex_ptr(iv1)
                  )
                ) {
                ++iv1;
            }
            if(iv1 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }
            
            iv2 = iv1 + 1;
            while(
                iv2 < nb_vertices_non_periodic_ &&
                PCK::points_are_colinear_3d(
                    non_periodic_vertex_ptr(iv0),
		    non_periodic_vertex_ptr(iv1),
		    non_periodic_vertex_ptr(iv2)
                  )
                ) {
                ++iv2;
            }
            if(iv2 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }

            iv3 = iv2 + 1;
            Sign s = ZERO;
            while(
                iv3 < nb_vertices_non_periodic_ &&
                (s = PCK::orient_3d(
                    non_periodic_vertex_ptr(iv0),
		    non_periodic_vertex_ptr(iv1),
                    non_periodic_vertex_ptr(iv2),
		    non_periodic_vertex_ptr(iv3)
                 )) == ZERO
            ) {
                ++iv3;
            }

            if(iv3 == nb_vertices()) {
                return NO_TETRAHEDRON;
            }

            geo_debug_assert(s != ZERO);
            
            if(s == NEGATIVE) {
                std::swap(iv2, iv3);
            }

            // Create the first tetrahedron
            index_t t0 = new_tetrahedron(
                signed_index_t(iv0), 
                signed_index_t(iv1), 
                signed_index_t(iv2), 
                signed_index_t(iv3)
            );

            // Create the first four virtual tetrahedra surrounding it
            index_t t[4];
            for(index_t f = 0; f < 4; ++f) {
                // In reverse order since it is an adjacent tetrahedron
                signed_index_t v1 = tet_vertex(t0, tet_facet_vertex(f,2));
                signed_index_t v2 = tet_vertex(t0, tet_facet_vertex(f,1));
                signed_index_t v3 = tet_vertex(t0, tet_facet_vertex(f,0));
                t[f] = new_tetrahedron(VERTEX_AT_INFINITY, v1, v2, v3);
            }

            // Connect the virtual tetrahedra to the real one
            for(index_t f=0; f<4; ++f) {
                set_tet_adjacent(t[f], 0, t0);
                set_tet_adjacent(t0, f, t[f]);
            }

            // Interconnect the four virtual tetrahedra along their common
            // faces
            for(index_t f = 0; f < 4; ++f) {
                // In reverse order since it is an adjacent tetrahedron
                index_t lv1 = tet_facet_vertex(f,2);
                index_t lv2 = tet_facet_vertex(f,1);
                index_t lv3 = tet_facet_vertex(f,0);
                set_tet_adjacent(t[f], 1, t[lv1]);
                set_tet_adjacent(t[f], 2, t[lv2]);
                set_tet_adjacent(t[f], 3, t[lv3]);
            }

            v1_ = iv0;
            v2_ = iv1;
            v3_ = iv2;
            v4_ = iv3;

            release_tets();

            return t0;
        }


	index_t stellate_cavity(index_t v) {
	    index_t new_tet = index_t(-1);

	    for(index_t f=0; f<cavity_.nb_facets(); ++f) {
		index_t old_tet = cavity_.facet_tet(f);
		index_t lf = cavity_.facet_facet(f);
		index_t t_neigh = index_t(tet_adjacent(old_tet, lf));
		signed_index_t v1 = cavity_.facet_vertex(f,0);
		signed_index_t v2 = cavity_.facet_vertex(f,1);
		signed_index_t v3 = cavity_.facet_vertex(f,2);
		new_tet = new_tetrahedron(signed_index_t(v), v1, v2, v3);
		set_tet_adjacent(new_tet, 0, t_neigh);
		set_tet_adjacent(t_neigh, find_tet_adjacent(t_neigh,old_tet), new_tet);
		cavity_.set_facet_tet(f, new_tet);
	    }
	
	    for(index_t f=0; f<cavity_.nb_facets(); ++f) {
		new_tet = cavity_.facet_tet(f);
		index_t neigh1, neigh2, neigh3;
		cavity_.get_facet_neighbor_tets(f, neigh1, neigh2, neigh3);
		set_tet_adjacent(new_tet, 1, neigh1);
		set_tet_adjacent(new_tet, 2, neigh2);
		set_tet_adjacent(new_tet, 3, neigh3);		
	    }
	    
	    return new_tet;
	}

	
        bool insert(index_t v, index_t& hint) {

            // If v is one of the vertices of the
            // first tetrahedron, nothing to do.
            if(
                v == v1_ ||
                v == v2_ ||
                v == v3_ ||
                v == v4_
            ) {
                return true;
            }

	    vec3 p = vertex(v);
	    
            Sign orient[4];
            index_t t = locate(v,p,hint,orient);

            //   locate() may fail due to tets already owned by
            // other threads.
            if(t == NO_TETRAHEDRON) {
                ++nb_failed_locate_;
                geo_debug_assert(nb_acquired_tets_ == 0);
                return false;
            }

            //  At this point, t is a valid tetrahedron,
            // and this thread acquired a lock on it.

            // Test whether the point already exists in
            // the triangulation. The point already exists
            // if it's located on three faces of the
            // tetrahedron returned by locate().
            int nb_zero = 
                (orient[0] == ZERO) +
                (orient[1] == ZERO) +
                (orient[2] == ZERO) +
                (orient[3] == ZERO) ;

            if(nb_zero >= 3) {
                release_tet(t);
                return true;
            }

            geo_debug_assert(nb_acquired_tets_ == 1);

            index_t t_bndry = NO_TETRAHEDRON;
            index_t f_bndry = index_t(-1);

	    vec4 p_lifted = lifted_vertex(v,p);

	    cavity_.clear();
	    
            bool ok = find_conflict_zone(v,p_lifted,t,t_bndry,f_bndry);

            // When in multithreading mode, we cannot allocate memory
            // dynamically and we use a fixed pool. If the fixed pool
            // is full, then we exit the thread (and the missing points
            // are inserted after, in sequential mode).
            if(
                nb_tets_to_create_ > nb_free_ &&
                Process::is_running_threads()
            ) {
                memory_overflow_ = true;
                ok = false;
            }

            if(!ok) {
                //  At this point, this thread did not successfully
                // acquire all the tets in the conflict zone, so
                // we need to rollback.
                release_tets();
                geo_debug_assert(nb_acquired_tets_ == 0);
                return false;
            }

            // The conflict list can be empty if
            //  the triangulation is weighted and v is not visible
            if(tets_to_delete_.size() == 0) {
                release_tets();
                geo_debug_assert(nb_acquired_tets_ == 0);
		has_empty_cells_ = true;
                return true;
            }


            geo_debug_assert(
                nb_acquired_tets_ == 
                tets_to_delete_.size() + tets_to_release_.size()
            );

#ifdef GEO_DEBUG
            // Sanity check: make sure this threads owns all the tets 
            // in conflict and their neighbors.
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                index_t tdel = tets_to_delete_[i];
                geo_debug_assert(owns_tet(tdel));
                for(index_t lf=0; lf<4; ++lf) {
                    geo_debug_assert(tet_adjacent(tdel,lf) >= 0);
                    geo_debug_assert(owns_tet(index_t(tet_adjacent(tdel,lf))));
                }
            }
#endif
            geo_debug_assert(owns_tet(t_bndry));
            geo_debug_assert(owns_tet(index_t(tet_adjacent(t_bndry,f_bndry))));
            geo_debug_assert(
                !tet_is_marked_as_conflict(
                    index_t(tet_adjacent(t_bndry,f_bndry))
                )
            );

            //   At this point, this threads owns all the tets in conflict and
            // their neighbors, therefore no other thread can interfere, and
            // we can update the triangulation.

            index_t new_tet = index_t(-1);
	    if(cavity_.OK()) {
		new_tet = stellate_cavity(v);	
	    } else {
		new_tet = stellate_conflict_zone_iterative(v,t_bndry,f_bndry);
	    }
       
            // Recycle the tetrahedra of the conflict zone.
            for(index_t i=0; i<tets_to_delete_.size()-1; ++i) {
                cell_next_[tets_to_delete_[i]] = tets_to_delete_[i+1];
            }
            cell_next_[tets_to_delete_[tets_to_delete_.size()-1]] =
                first_free_;
            first_free_ = tets_to_delete_[0];
            nb_free_ += nb_tets_in_conflict();

	    // Reset deleted tet.
	    // This is needed because we update_v_to_cell() in
	    // a transient state.
	    // Note: update_v_to_cell() is overloaded here,
	    // with a check on nb_vertices_non_periodic_,
	    // this is why the (-2) does not make everything crash.
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                index_t tdel = tets_to_delete_[i];
                set_tet_vertex(tdel,0,-2);
                set_tet_vertex(tdel,1,-2);
                set_tet_vertex(tdel,2,-2);
                set_tet_vertex(tdel,3,-2);
            }
       
            // Return one of the newly created tets
            hint=new_tet;
            release_tets();
            geo_debug_assert(nb_acquired_tets_ == 0);
            return true;
        }

        bool find_conflict_zone(
            index_t v, const vec4& p, index_t t, 
            index_t& t_bndry, index_t& f_bndry
        ) {
            nb_tets_to_create_ = 0;

            geo_debug_assert(t != NO_TETRAHEDRON);
            geo_debug_assert(owns_tet(t));

            // Pointer to the coordinates of the point to be inserted
            //const double* p = vertex_ptr(v);

            //  Weighted triangulations can have dangling
            // vertices. Such vertices p are characterized by
            // the fact that p is not in conflict with the 
            // tetrahedron returned by locate().
            if(!tet_is_in_conflict(t,v,p)) {
                release_tet(t);
                return true;
            }

            mark_tet_as_conflict(t);

            //   Sanity check: the vertex to be inserted should
            // not correspond to one of the vertices of t.
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,0));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,1));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,2));
            geo_debug_assert(signed_index_t(v) != tet_vertex(t,3));

            // Note: points on edges and on facets are
            // handled by the way tet_is_in_conflict()
            // is implemented, that naturally inserts
            // the correct tetrahedra in the conflict list.

            // Determine the conflict list by greedy propagation from t.
            bool result = find_conflict_zone_iterative(v, p,t);
            t_bndry = t_boundary_;
            f_bndry = f_boundary_;
            return result;
        }


        bool find_conflict_zone_iterative(
            index_t v_in, const vec4& p_in, index_t t_in
        ) {
            geo_debug_assert(owns_tet(t_in));
            S_.push_back(SFrame(t_in, v_in, p_in));

            while(S_.size() != 0) {
                index_t t = S_.rbegin()->t;
		index_t v = S_.rbegin()->v;
		vec4    p = S_.rbegin()->p;
                S_.pop_back();

                geo_debug_assert(owns_tet(t));

                for(index_t lf = 0; lf < 4; ++lf) {
		    index_t v2=v;
		    vec4 p2=p;
		    
                    index_t t2 = index_t(tet_adjacent(t, lf));
                
                    // If t2 is already owned by current thread, then
                    // its status was previously determined.
                    if(owns_tet(t2)) {

                        geo_debug_assert(
                            tet_is_marked_as_conflict(t2) == 
                            tet_is_in_conflict(t2,v2,p2)
                        );
                    
                        // If t2 is not in conflict list, then t has a facet
                        // on the border of the conflict zone, and there is
                        // a tet to create.
                        if(!tet_is_marked_as_conflict(t2)) {
                            ++nb_tets_to_create_;
			    cavity_.new_facet(
				t, lf,
				tet_vertex(t, tet_facet_vertex(lf,0)),
				tet_vertex(t, tet_facet_vertex(lf,1)),
				tet_vertex(t, tet_facet_vertex(lf,2))
			    );
                        }
                        continue;
                    }

                    if(!acquire_tet(t2)) {
                        S_.resize(0);
                        return false;
                    }
                
                    geo_debug_assert(owns_tet(t2));

                    if(!tet_is_in_conflict(t2,v2,p2)) {
                        mark_tet_as_neighbor(t2);
                        // If t2 is not in conflict list, then t has a facet
                        // on the border of the conflict zone, and there is
                        // a tet to create.
                        ++nb_tets_to_create_;
                    } else {
                        mark_tet_as_conflict(t2);
                        geo_debug_assert(owns_tet(t2));
                        S_.push_back(SFrame(t2,v2,p2));
                        continue;
                    }

                    //  At this point, t is in conflict 
                    // and t2 is not in conflict. 
                    // We keep a reference to a tet on the boundary
                    t_boundary_ = t;
                    f_boundary_ = lf;
		    cavity_.new_facet(
			t, lf,
			tet_vertex(t, tet_facet_vertex(lf,0)),
			tet_vertex(t, tet_facet_vertex(lf,1)),
			tet_vertex(t, tet_facet_vertex(lf,2))
		    );
                    geo_debug_assert(tet_adjacent(t,lf) == signed_index_t(t2));
                    geo_debug_assert(owns_tet(t));
                    geo_debug_assert(owns_tet(t2));
                }
            }
            return true;
        }

        const double* non_periodic_vertex_ptr(index_t i) const {
            geo_debug_assert(i < nb_vertices_non_periodic_);
	    return vertices_ + i*3;
        }

	double non_periodic_weight(index_t i) const {
            geo_debug_assert(i < nb_vertices_non_periodic_);
	    return (weights_ == nullptr) ? 0.0 : weights_[i];
        }

	
	void get_vertex(index_t v, double* result) const {
	    if(!periodic_) {
		result[0] = vertices_[3*v];
		result[1] = vertices_[3*v+1];
		result[2] = vertices_[3*v+2];
		return;
	    }
	    index_t instance = periodic_vertex_instance(v);
	    v = periodic_vertex_real(v);
	    result[0] = vertices_[3*v];
	    result[1] = vertices_[3*v+1];
	    result[2] = vertices_[3*v+2];
	    result[0] += double(translation[instance][0]) * period_;
	    result[1] += double(translation[instance][1]) * period_;
	    result[2] += double(translation[instance][2]) * period_;
	}
	
	vec3 vertex(index_t v) const {
	    vec3 result;
	    get_vertex(v, result.data());
	    return result;
	}

	void get_lifted_vertex(index_t v, double* result) const {
	    index_t instance = 0;
	    if(periodic_) {
		instance = periodic_vertex_instance(v);
		v = periodic_vertex_real(v);
	    }
	    result[0] = vertices_[3*v];
	    result[1] = vertices_[3*v+1];
	    result[2] = vertices_[3*v+2];
	    result[3] = -non_periodic_weight(v);
	    if(periodic_) {
		result[0] += double(translation[instance][0]) * period_;
		result[1] += double(translation[instance][1]) * period_;
		result[2] += double(translation[instance][2]) * period_;
	    }
	    result[3] +=
		geo_sqr(result[0]) + geo_sqr(result[1]) + geo_sqr(result[2]);
	}
	
	vec4 lifted_vertex(index_t v) const {
	    vec4 result;
	    get_lifted_vertex(v,result.data());
	    return result;
	}

	vec4 lifted_vertex(index_t v, const vec3& p) {
	    return vec4(
		p.x, p.y, p.z,
		geo_sqr(p.x) + geo_sqr(p.y) + geo_sqr(p.z) - non_periodic_weight(periodic_vertex_real(v))
	    );
	}
	
	Sign orient_3d(index_t i, index_t j, index_t k, index_t l) const {
	    // No need to reorder since there is no SOS.
	    double V[4][3];
	    get_vertex(i,V[0]);
	    get_vertex(j,V[1]);
	    get_vertex(k,V[2]);
	    get_vertex(l,V[3]);
	    return PCK::orient_3d(V[0], V[1], V[2], V[3]);
	}

	Sign in_circle_3dlifted_SOS(index_t i, index_t j, index_t k, index_t l) const {

	    // In non-periodic mode, directly access vertices.
	    if(!periodic_) {
		const double* pi = non_periodic_vertex_ptr(i);
		const double* pj = non_periodic_vertex_ptr(j);
		const double* pk = non_periodic_vertex_ptr(k);
		const double* pl = non_periodic_vertex_ptr(l);		
		double hi = geo_sqr(pi[0]) + geo_sqr(pi[1]) + geo_sqr(pi[2]) - non_periodic_weight(i);
		double hj = geo_sqr(pj[0]) + geo_sqr(pj[1]) + geo_sqr(pj[2]) - non_periodic_weight(j);
		double hk = geo_sqr(pk[0]) + geo_sqr(pk[1]) + geo_sqr(pk[2]) - non_periodic_weight(k);
		double hl = geo_sqr(pl[0]) + geo_sqr(pl[1]) + geo_sqr(pl[2]) - non_periodic_weight(l);
		return PCK::in_circle_3dlifted_SOS(
		    pi, pj, pk, pl,
		    hi, hj, hk, hl
		);
	    }
	    
	    double V[4][4];
	    get_lifted_vertex(i,V[0]);
	    get_lifted_vertex(j,V[1]);
	    get_lifted_vertex(k,V[2]);
	    get_lifted_vertex(l,V[3]);
	    return PCK::in_circle_3dlifted_SOS(
		V[0],    V[1],    V[2],    V[3],
		V[0][3], V[1][3], V[2][3], V[3][3]
	    );
	}

	Sign orient_3dlifted_SOS(index_t i, index_t j, index_t k, index_t l, index_t m) const {

	    // In non-periodic mode, directly access vertices.
	    if(!periodic_) {
		const double* pi = non_periodic_vertex_ptr(i);
		const double* pj = non_periodic_vertex_ptr(j);
		const double* pk = non_periodic_vertex_ptr(k);
		const double* pl = non_periodic_vertex_ptr(l);		
		const double* pm = non_periodic_vertex_ptr(m);
		double hi = geo_sqr(pi[0]) + geo_sqr(pi[1]) + geo_sqr(pi[2]) - non_periodic_weight(i);
		double hj = geo_sqr(pj[0]) + geo_sqr(pj[1]) + geo_sqr(pj[2]) - non_periodic_weight(j);
		double hk = geo_sqr(pk[0]) + geo_sqr(pk[1]) + geo_sqr(pk[2]) - non_periodic_weight(k);
		double hl = geo_sqr(pl[0]) + geo_sqr(pl[1]) + geo_sqr(pl[2]) - non_periodic_weight(l);
		double hm = geo_sqr(pm[0]) + geo_sqr(pm[1]) + geo_sqr(pm[2]) - non_periodic_weight(m);		
		return PCK::orient_3dlifted_SOS(
		    pi, pj, pk, pl, pm,
		    hi, hj, hk, hl, hm
		);
	    }

	    // Periodic mode.
	    double V[5][4];	    
	    get_lifted_vertex(i,V[0]);
	    get_lifted_vertex(j,V[1]);
	    get_lifted_vertex(k,V[2]);
	    get_lifted_vertex(l,V[3]);
	    get_lifted_vertex(m,V[4]);
	    return PCK::orient_3dlifted_SOS(
		V[0],    V[1],    V[2],    V[3],    V[4],
		V[0][3], V[1][3], V[2][3], V[3][3], V[4][3]
	    );
	}
	
        bool tet_is_in_conflict(index_t t, index_t v, const vec4& p) const {

	    geo_argused(p);
	    
            // Lookup tetrahedron vertices

	    index_t iv[4];
            for(index_t i=0; i<4; ++i) {
                iv[i] = index_t(tet_vertex(t,i));
	    }

	    
            // Check for virtual tetrahedra (then in_sphere()
            // is replaced with orient3d())
            for(index_t lf = 0; lf < 4; ++lf) {

                if(iv[lf] == index_t(-1)) {

                    // Facet of a virtual tetrahedron opposite to
                    // infinite vertex corresponds to
                    // the triangle on the convex hull of the points.
                    // Orientation is obtained by replacing vertex lf
                    // with p.
                    iv[lf] = v;

		    // no SOS, we can directly use the PCK predicate
                    Sign sign = orient_3d(iv[0], iv[1], iv[2], iv[3]);

                    if(sign > 0) {
                        return true;
                    }

                    if(sign < 0) {
                        return false;
                    }

                    // If sign is zero, we check the real tetrahedron
                    // adjacent to the facet on the convex hull.
                    geo_debug_assert(tet_adjacent(t, lf) >= 0);
                    index_t t2 = index_t(tet_adjacent(t, lf));
                    geo_debug_assert(!tet_is_virtual(t2));

                    //   If t2 was already visited by this thread, then
                    // it is in conflict if it is already marked.
                    if(owns_tet(t2)) {
                        return tet_is_marked_as_conflict(t2);
                    }
                    
                    //  If t2 was not already visited, then we need to
                    // switch to the in_circum_circle_3d() predicate.

                    index_t iq0 = iv[(lf+1)%4];
                    index_t iq1 = iv[(lf+2)%4];
                    index_t iq2 = iv[(lf+3)%4];
                    
		    return (in_circle_3dlifted_SOS(iq0, iq1, iq2, v) > 0);
                }
            }

            //   If the tetrahedron is a finite one, it is in conflict
            // if its circumscribed sphere contains the point (this is
            // the standard case).

	    return (orient_3dlifted_SOS(iv[0], iv[1], iv[2], iv[3], v) > 0);
        }
        

         index_t locate(
	     index_t& v, vec3& p, index_t hint = NO_TETRAHEDRON,
	     Sign* orient = nullptr
         ) {
	     geo_argused(v);
	     nb_traversed_tets_ = 0;

             // If no hint specified, find a tetrahedron randomly

             if(hint != NO_TETRAHEDRON) {
                 if(tet_is_free(hint)) {
                     hint = NO_TETRAHEDRON;
                 } else {
                     if( !owns_tet(hint) && !acquire_tet(hint) ) {
                         hint = NO_TETRAHEDRON;
                     }
                     if((hint != NO_TETRAHEDRON) && tet_is_free(hint)) {
                         release_tet(hint);
                         hint = NO_TETRAHEDRON;
                     }
                 }
             }

             do {
                 if(hint == NO_TETRAHEDRON) {
                     hint = thread_safe_random_(max_used_t_);
                 }
                 if(
                     tet_is_free(hint) || 
                     (!owns_tet(hint) && !acquire_tet(hint))
                 ) {
                     if(owns_tet(hint)) {
                         release_tet(hint);
                     }
                     hint = NO_TETRAHEDRON;
                 } else {
                     for(index_t f=0; f<4; ++f) {
                         if(tet_vertex(hint,f) == VERTEX_AT_INFINITY) {
                             index_t new_hint = index_t(tet_adjacent(hint,f));
                             if(
                                 tet_is_free(new_hint) || 
                                 !acquire_tet(new_hint)
                             ) {
                                 new_hint = NO_TETRAHEDRON;
                             }
                             release_tet(hint);
                             hint = new_hint;
                             break;
                         }
                     }
                 }
             } while(hint == NO_TETRAHEDRON) ;

             index_t t = hint;
             index_t t_pred = NO_TETRAHEDRON;
             Sign orient_local[4];
             if(orient == nullptr) {
                 orient = orient_local;
             }


         still_walking:
             {
                 if(t_pred != NO_TETRAHEDRON) {
                     release_tet(t_pred);
                 }

                 if(tet_is_free(t)) {
                     return NO_TETRAHEDRON;
                 }

                 if(!owns_tet(t) && !acquire_tet(t)) {
                     return NO_TETRAHEDRON;
                 }


                 if(!tet_is_real(t)) {
                     release_tet(t);
                     return NO_TETRAHEDRON;
                 }

                 vec3 pv[4];
                 pv[0] = vertex(finite_tet_vertex(t,0));
                 pv[1] = vertex(finite_tet_vertex(t,1));
                 pv[2] = vertex(finite_tet_vertex(t,2));
                 pv[3] = vertex(finite_tet_vertex(t,3));

                 // Start from a random facet
                 index_t f0 = thread_safe_random_4_();
                 for(index_t df = 0; df < 4; ++df) {
                     index_t f = (f0 + df) % 4;
                     
                     signed_index_t s_t_next = tet_adjacent(t,f);
                     
                     //  If the opposite tet is -1, then it means that
                     // we are trying to locate() (e.g. called from
                     // nearest_vertex) within a tetrahedralization 
                     // from which the infinite tets were removed.
                     if(s_t_next == -1) {
                         release_tet(t);
                         return NO_TETRAHEDRON;
                     }
                     
                     index_t t_next = index_t(s_t_next);
                     
                     //   If the candidate next tetrahedron is the
                     // one we came from, then we know already that
                     // the orientation is positive, thus we examine
                     // the next candidate (or exit the loop if they
                     // are exhausted).
                     if(t_next == t_pred) {
                         orient[f] = POSITIVE ;
                         continue ; 
                     }

                     //   To test the orientation of p w.r.t. the facet f of
                     // t, we replace vertex number f with p in t (same
                     // convention as in CGAL).
                     // This is equivalent to tet_facet_point_orient3d(t,f,p)
                     // (but less costly, saves a couple of lookups)
                     vec3 pv_bkp = pv[f];
                     pv[f] = p;
                     orient[f] = PCK::orient_3d(
			 pv[0].data(), pv[1].data(), pv[2].data(), pv[3].data()
		     );
                     
                     //   If the orientation is not negative, then we cannot
                     // walk towards t_next, and examine the next candidate
                     // (or exit the loop if they are exhausted).
                     if(orient[f] != NEGATIVE) {
                         pv[f] = pv_bkp;
                         continue;
                     }

                     //  If the opposite tet is a virtual tet, then
                     // the point has a positive orientation relative
                     // to the facet on the border of the convex hull,
                     // thus t_next is a tet in conflict and we are
                     // done.
                     if(tet_is_virtual(t_next)) {
                         release_tet(t);
                         if(!acquire_tet(t_next)) {
                             return NO_TETRAHEDRON;
                         }
                         for(index_t lf = 0; lf < 4; ++lf) {
                             orient[lf] = POSITIVE;
                         }
                         return t_next;
                     }

		     ++nb_traversed_tets_;
		     
                     //   If we reach this point, then t_next is a valid
                     // successor, thus we are still walking.
                     t_pred = t;
                     t = t_next;
                     goto still_walking;
                 }
             } 

             //   If we reach this point, we did not find a valid successor
             // for walking (a face for which p has negative orientation), 
             // thus we reached the tet for which p has all positive 
             // face orientations (i.e. the tet that contains p).

#ifdef GEO_DEBUG
             geo_debug_assert(tet_is_real(t));

             vec3 pv[4];
             Sign signs[4];
             pv[0] = vertex(finite_tet_vertex(t,0));
             pv[1] = vertex(finite_tet_vertex(t,1));
             pv[2] = vertex(finite_tet_vertex(t,2));
             pv[3] = vertex(finite_tet_vertex(t,3));
             for(index_t f=0; f<4; ++f) {
                 vec3 pv_bkp = pv[f];
                 pv[f] = vec3(p.x, p.y, p.z);
                 signs[f] = PCK::orient_3d(pv[0].data(), pv[1].data(), pv[2].data(), pv[3].data());
                 geo_debug_assert(signs[f] >= 0);
                 pv[f] = pv_bkp;
             }
#endif

             return t;
         }
        

    protected:
        
        bool tet_is_marked_as_conflict(index_t t) const {
            geo_debug_assert(owns_tet(t));
            return ((cell_thread_[t] & 1) != 0);
        }


        index_t nb_tets_in_conflict() const {
            return tets_to_delete_.size();
        }

        void mark_tet_as_conflict(index_t t) {
            geo_debug_assert(owns_tet(t));
            tets_to_delete_.push_back(t);
            cell_thread_[t] |= 1;
            geo_debug_assert(owns_tet(t));
            geo_debug_assert(tet_is_marked_as_conflict(t));
        }

        void mark_tet_as_neighbor(index_t t) {
            //   Note: nothing to change in cell_thread_[t]
            // since LSB=0 means neigbhor tet.
            tets_to_release_.push_back(t);
        }

        void acquire_and_mark_tet_as_created(index_t t) {
            //  The tet was created in this thread's tet pool,
            // therefore there is no need to use sync 
            // primitives to acquire a lock on it.
            geo_debug_assert(cell_thread_[t] == NO_THREAD);
            cell_thread_[t] = thread_index_t(id() << 1);
#ifdef GEO_DEBUG
            ++nb_acquired_tets_;
#endif
            tets_to_release_.push_back(t);            
        }


        void release_tets() {
            for(index_t i=0; i<tets_to_release_.size(); ++i) {
                release_tet(tets_to_release_[i]);
            }
            tets_to_release_.resize(0);
            for(index_t i=0; i<tets_to_delete_.size(); ++i) {
                release_tet(tets_to_delete_[i]);
            }
            tets_to_delete_.resize(0);
        }

        bool acquire_tet(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!owns_tet(t));

#if defined(GEO_COMPILER_MSVC)
           // Note: comparand and exchange parameter are swapped in Windows API
           // as compared to __sync_val_compare_and_swap !!
            interfering_thread_ =
                (thread_index_t)(_InterlockedCompareExchange8(
                    (volatile char *)(&cell_thread_[t]),
                    (char)(id() << 1),
                    (char)(NO_THREAD)
                ));
#else            
            interfering_thread_ = 
                __sync_val_compare_and_swap(
                    &cell_thread_[t], NO_THREAD, thread_index_t(id() << 1)
                );
#endif
            
            if(interfering_thread_ == NO_THREAD) {
                geo_debug_assert(t == first_free_ || !tet_is_in_list(t));
#ifdef GEO_DEBUG
                ++nb_acquired_tets_;
#endif
                return true;
            }
            return false;
        }

        void release_tet(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(owns_tet(t));
#ifdef GEO_DEBUG
            --nb_acquired_tets_;
#endif
            cell_thread_[t] = NO_THREAD;
        }


        bool owns_tet(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_thread_[t] >> 1) == thread_index_t(id());
        }

        bool tet_is_virtual(index_t t) const {
            return
                !tet_is_free(t) && (
                cell_to_v_store_[4 * t] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 1] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 2] == VERTEX_AT_INFINITY ||
                cell_to_v_store_[4 * t + 3] == VERTEX_AT_INFINITY) ;
        }

        
        static index_t tet_facet_vertex(index_t f, index_t v) {
            geo_debug_assert(f < 4);
            geo_debug_assert(v < 3);
            return index_t(tet_facet_vertex_[f][v]);
        }

        signed_index_t tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            return cell_to_v_store_[4 * t + lv];
        }

        index_t find_tet_vertex(index_t t, signed_index_t v) const {
            geo_debug_assert(t < max_t());
            //   Find local index of v in tetrahedron t vertices.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);
            return find_4(T,v);
        }


         index_t finite_tet_vertex(index_t t, index_t lv) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            geo_debug_assert(cell_to_v_store_[4 * t + lv] != -1);
            return index_t(cell_to_v_store_[4 * t + lv]);
        }

        void set_tet_vertex(index_t t, index_t lv, signed_index_t v) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lv < 4);
            geo_debug_assert(owns_tet(t));
            cell_to_v_store_[4 * t + lv] = v;
        }

        signed_index_t tet_adjacent(index_t t, index_t lf) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(lf < 4);
            signed_index_t result = cell_to_cell_store_[4 * t + lf];
            return result;
        }

        void set_tet_adjacent(index_t t1, index_t lf1, index_t t2) {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2 < max_t());
            geo_debug_assert(lf1 < 4);
            geo_debug_assert(owns_tet(t1));
            geo_debug_assert(owns_tet(t2));
            cell_to_cell_store_[4 * t1 + lf1] = signed_index_t(t2);
        }
        
        index_t find_tet_adjacent(
            index_t t1, index_t t2_in
        ) const {
            geo_debug_assert(t1 < max_t());
            geo_debug_assert(t2_in < max_t());
            geo_debug_assert(t1 != t2_in);

            signed_index_t t2 = signed_index_t(t2_in);

            // Find local index of t2 in tetrahedron t1 adajcent tets.
            const signed_index_t* T = &(cell_to_cell_store_[4 * t1]);
            index_t result = find_4(T,t2);

            // Sanity check: make sure that t1 is adjacent to t2
            // only once!
            geo_debug_assert(tet_adjacent(t1,(result+1)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+2)%4) != t2);
            geo_debug_assert(tet_adjacent(t1,(result+3)%4) != t2);
            return result;
        }

        index_t get_facet_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);
            //   Find local index of v1 and v2 in tetrahedron t
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);

	    index_t lv1, lv2;
	    lv1 = find_4(T,v1);
	    lv2 = find_4(T,v2);
            geo_debug_assert(lv1 != lv2);
            return index_t(halfedge_facet_[lv1][lv2]);
        }
        

        void get_facets_by_halfedge(
            index_t t, signed_index_t v1, signed_index_t v2,
            index_t& f12, index_t& f21
        ) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(v1 != v2);
        
            //   Find local index of v1 and v2 in tetrahedron t
            // The following expression is 10% faster than using
            // if() statements (multiply by boolean result of test).
            // Thank to Laurent Alonso for this idea.
            const signed_index_t* T = &(cell_to_v_store_[4 * t]);

	    signed_index_t lv1,lv2;

	    lv1 = (T[1] == v1) | ((T[2] == v1) * 2) | ((T[3] == v1) * 3);
	    lv2 = (T[1] == v2) | ((T[2] == v2) * 2) | ((T[3] == v2) * 3);
	    geo_debug_assert(lv1 != 0 || T[0] == v1);
	    geo_debug_assert(lv2 != 0 || T[0] == v2);
            
            geo_debug_assert(lv1 >= 0);
            geo_debug_assert(lv2 >= 0);
            geo_debug_assert(lv1 != lv2);

            f12 = index_t(halfedge_facet_[lv1][lv2]);
            f21 = index_t(halfedge_facet_[lv2][lv1]);
        }

        static const index_t END_OF_LIST = index_t(-1);


        static const index_t NOT_IN_LIST = index_t(-2);

        index_t nb_vertices() const {
            return nb_vertices_;
        }

        bool tet_is_in_list(index_t t) const {
            geo_debug_assert(t < max_t());
            return (cell_next_[t] != NOT_IN_LIST);
        }

        index_t tet_next(index_t t) const {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            return cell_next_[t];
        }


        index_t tet_thread(index_t t) const {
            geo_debug_assert(t < max_t());
            return cell_thread_[t];
        }

        void add_tet_to_list(index_t t, index_t& first, index_t& last) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(!tet_is_in_list(t));
            geo_debug_assert(owns_tet(t));
            if(last == END_OF_LIST) {
                geo_debug_assert(first == END_OF_LIST);
                first = last = t;
                cell_next_[t] = END_OF_LIST;
            } else {
                cell_next_[t] = first;
                first = t;
            }
        }

        void remove_tet_from_list(index_t t) {
            geo_debug_assert(t < max_t());
            geo_debug_assert(tet_is_in_list(t));
            geo_debug_assert(owns_tet(t));
            cell_next_[t] = NOT_IN_LIST;
        }


        index_t new_tetrahedron() {
            // If the memory pool is full, then we expand it.
            // This cannot be done when running multiple threads.
            if(first_free_ == END_OF_LIST) {
                geo_debug_assert(!Process::is_running_threads());

		if(
		    master_->cell_to_v_store_.size() ==
		    master_->cell_to_v_store_.capacity()
		) {
		    master_->nb_reallocations_++;
		}
		
                master_->cell_to_v_store_.resize(
                    master_->cell_to_v_store_.size() + 4, -1
                );
                master_->cell_to_cell_store_.resize(
                    master_->cell_to_cell_store_.size() + 4, -1
                );
                // index_t(NOT_IN_LIST) is necessary, else with
                // NOT_IN_LIST alone the compiler tries to generate a
                // reference to NOT_IN_LIST resulting in a link error.
                master_->cell_next_.push_back(index_t(END_OF_LIST));
                master_->cell_thread_.push_back(thread_index_t(NO_THREAD));
                ++nb_free_;
                ++max_t_;
                first_free_ = master_->cell_thread_.size() - 1;
            }

            acquire_and_mark_tet_as_created(first_free_);
            index_t result = first_free_;

            first_free_ = tet_next(first_free_);
            remove_tet_from_list(result);

            cell_to_cell_store_[4 * result] = -1;
            cell_to_cell_store_[4 * result + 1] = -1;
            cell_to_cell_store_[4 * result + 2] = -1;
            cell_to_cell_store_[4 * result + 3] = -1;

            max_used_t_ = std::max(max_used_t_, result);

            --nb_free_;
            return result;
        }

        index_t new_tetrahedron(
            signed_index_t v1, signed_index_t v2, 
            signed_index_t v3, signed_index_t v4
        ) {
            index_t result = new_tetrahedron();
            cell_to_v_store_[4 * result] = v1;
            cell_to_v_store_[4 * result + 1] = v2;
            cell_to_v_store_[4 * result + 2] = v3;
            cell_to_v_store_[4 * result + 3] = v4;
            return result;
        }

        static index_t find_4(const signed_index_t* T, signed_index_t v) {
            // The following expression is 10% faster than using
            // if() statements. This uses the C++ norm, that 
            // ensures that the 'true' boolean value converted to 
            // an int is always 1. With most compilers, this avoids 
            // generating branching instructions.
            // Thank to Laurent Alonso for this idea.
            // Note: Laurent also has this version:
            //    (T[0] != v)+(T[2]==v)+2*(T[3]==v)
            // that avoids a *3 multiply, but it is not faster in
            // practice.
            index_t result = index_t(
                (T[1] == v) | ((T[2] == v) * 2) | ((T[3] == v) * 3)
            );
            // Sanity check, important if it was T[0], not explicitly
            // tested (detects input that does not meet the precondition).
            geo_debug_assert(T[result] == v);
            return result; 
        }


	 index_t find_4_periodic(const signed_index_t* T, index_t v) const {

	     // v needs to be a real vertex.
	     geo_debug_assert(periodic_vertex_instance(v) == 0);

	     geo_debug_assert(T[0] != -1 && T[1] != -1 && T[2] != -1 && T[3] != -1);
	     
            // The following expression is 10% faster than using
            // if() statements. This uses the C++ norm, that 
            // ensures that the 'true' boolean value converted to 
            // an int is always 1. With most compilers, this avoids 
            // generating branching instructions.
            // Thank to Laurent Alonso for this idea.
            // Note: Laurent also has this version:
            //    (T[0] != v)+(T[2]==v)+2*(T[3]==v)
            // that avoids a *3 multiply, but it is not faster in
            // practice.
            index_t result = index_t(
                 (periodic_vertex_real(index_t(T[1])) == v)      |
		((periodic_vertex_real(index_t(T[2])) == v) * 2) |
		((periodic_vertex_real(index_t(T[3])) == v) * 3)
	    );
    
            // Sanity check, important if it was T[0], not explicitly
            // tested (detects input that does not meet the precondition).
	    geo_debug_assert(periodic_vertex_real(index_t(T[result])) == v);
            return result; 
        }

	
        void send_event() {
            pthread_cond_broadcast(&cond_);
        }
        
        void wait_for_event(index_t t) {
	    // Fixed by Hiep Vu: enlarged critical section (contains
	    // now the test (!thrd->finished)
            PeriodicDelaunay3dThread* thrd = thread(t);
	    pthread_mutex_lock(&(thrd->mutex_));	    
            if(!thrd->finished_) {
                pthread_cond_wait(&(thrd->cond_), &(thrd->mutex_));
            }
	    pthread_mutex_unlock(&(thrd->mutex_));	    
        }

                

        class StellateConflictStack {
        public:

            void push(index_t t1, index_t t1fbord, index_t t1fprev) {
                store_.resize(store_.size()+1);
                top().t1 = t1;
                top().t1fbord = Numeric::uint8(t1fbord);
                top().t1fprev = Numeric::uint8(t1fprev);
            }

            void save_locals(index_t new_t, index_t t1ft2, index_t t2ft1) {
                geo_debug_assert(!empty());
                top().new_t = new_t;
                top().t1ft2 = Numeric::uint8(t1ft2);
                top().t2ft1 = Numeric::uint8(t2ft1);
            }

            void get_parameters(
                index_t& t1, index_t& t1fbord, index_t& t1fprev
            ) const {
                geo_debug_assert(!empty());
                t1      = top().t1;
                t1fbord = index_t(top().t1fbord);
                t1fprev = index_t(top().t1fprev);
            }


            void get_locals(
                index_t& new_t, index_t& t1ft2, index_t& t2ft1
            ) const {
                geo_debug_assert(!empty());
                new_t = top().new_t;
                t1ft2 = index_t(top().t1ft2);
                t2ft1 = index_t(top().t2ft1);
            }
            
            void pop() {
                geo_debug_assert(!empty());
                store_.pop_back();
            }
            
            bool empty() const {
                return store_.empty();
            }
            
        private:

            struct Frame {
                // Parameters
                index_t t1;
                index_t new_t;                
                Numeric::uint8 t1fbord ;
                
                // Local variables
                Numeric::uint8 t1fprev ;
                Numeric::uint8 t1ft2   ;
                Numeric::uint8 t2ft1   ;
            };

            Frame& top() {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }

            const Frame& top() const {
                geo_debug_assert(!empty());
                return *store_.rbegin();
            }
            
            std::vector<Frame> store_;
        };

        index_t stellate_conflict_zone_iterative(
            index_t v_in, index_t t1, index_t t1fbord,
            index_t t1fprev = index_t(-1)
        ) {
            //   This function is de-recursified because some degenerate
            // inputs can cause stack overflow (system stack is limited to
            // a few megs). For instance, it can happen when a large number
            // of points are on the same sphere exactly.
            
            //   To de-recursify, it uses class StellateConflictStack
            // that emulates system's stack for storing functions's
            // parameters and local variables in all the nested stack
            // frames. 
        
            signed_index_t v = signed_index_t(v_in);
            
            S2_.push(t1, t1fbord, t1fprev);
            
            index_t new_t;   // the newly created tetrahedron.
            
            index_t t1ft2;   // traverses the 4 facets of t1.
            
            index_t t2;      // the tetrahedron on the border of
                             // the conflict zone that shares an
                             // edge with t1 along t1ft2.
            
            index_t t2fbord; // the facet of t2 on the border of
                             // the conflict zone.
        
            index_t t2ft1;   // the facet of t2 that is incident to t1.
        
        entry_point:
            S2_.get_parameters(t1, t1fbord, t1fprev);


            geo_debug_assert(owns_tet(t1));
            geo_debug_assert(tet_adjacent(t1,t1fbord)>=0);
            geo_debug_assert(owns_tet(index_t(tet_adjacent(t1,t1fbord))));
            geo_debug_assert(tet_is_marked_as_conflict(t1));
            geo_debug_assert(
                !tet_is_marked_as_conflict(index_t(tet_adjacent(t1,t1fbord)))
            );

            // Create new tetrahedron with same vertices as t_bndry

	    new_t = new_tetrahedron(
		tet_vertex(t1,0),
		tet_vertex(t1,1),
		tet_vertex(t1,2),
		tet_vertex(t1,3)
	    );

	    index_t tbord = index_t(tet_adjacent(t1,t1fbord));

	    // We generate the tetrahedron with the three vertices of the tet outside
	    // the conflict zone and the newly created vertex in the local frame of the
	    // tet outside the conflict zone.
	    
	    // Replace in new_t the vertex opposite to t1fbord with v		
	    set_tet_vertex(new_t, t1fbord, v);		

            {
		// Connect new_t with t1's neighbor across t1fbord		
                set_tet_adjacent(new_t, t1fbord, tbord);
                set_tet_adjacent(tbord, find_tet_adjacent(tbord,t1), new_t);
            }
            
            //  Lookup new_t's neighbors across its three other
            // facets and connect them
            for(t1ft2=0; t1ft2<4; ++t1ft2) {
                
                if(t1ft2 == t1fprev || tet_adjacent(new_t,t1ft2) != -1) {
                    continue;
                }
                
                // Get t1's neighbor along the border of the conflict zone
                if(!get_neighbor_along_conflict_zone_border(
                       t1,t1fbord,t1ft2, t2,t2fbord,t2ft1
                )) {
                    //   If t1's neighbor is not a new tetrahedron,
                    // create a new tetrahedron through a recursive call.
                    S2_.save_locals(new_t, t1ft2, t2ft1);
                    S2_.push(t2, t2fbord, t2ft1);
                    goto entry_point;

                return_point:
                    // This is the return value of the called function.
                    index_t result = new_t;
                    S2_.pop();
                    
                    // Special case: we were in the outermost frame, 
                    // then we (truly) return from the function.
                    if(S2_.empty()) {
                        return result;
                    }
                    
                    S2_.get_parameters(t1, t1fbord, t1fprev);
                    S2_.get_locals(new_t, t1ft2, t2ft1); 
                    t2 = result; 
                }
                
                set_tet_adjacent(t2, t2ft1, new_t);
                set_tet_adjacent(new_t, t1ft2, t2);
            }

            // Except for the initial call (see "Special case" above),
            // the nested calls all come from the same location,
            // thus there is only one possible return point
            // (no need to push any return address).
            goto return_point;
        }

        bool get_neighbor_along_conflict_zone_border(
            index_t t1,
            index_t t1fborder,
            index_t t1ft2,
            index_t& t2,
            index_t& t2fborder,
            index_t& t2ft1
        ) const {
            
            // Note: this function is a bit long for an inline function,
            // but I observed a (modest) performance gain doing so.
            
            //   Find two vertices that are both on facets new_f and f1
            //  (the edge around which we are turning)
            //  This uses duality as follows:
            //  Primal form (not used here): 
            //    halfedge_facet_[v1][v2] returns a facet that is incident
            //    to both v1 and v2.
            //  Dual form (used here):
            //    halfedge_facet_[f1][f2] returns a vertex that both 
            //    f1 and f2 are incident to.
            signed_index_t ev1 = 
                tet_vertex(t1, index_t(halfedge_facet_[t1ft2][t1fborder]));
            signed_index_t ev2 = 
                tet_vertex(t1, index_t(halfedge_facet_[t1fborder][t1ft2]));
            
            //   Turn around edge [ev1,ev2] inside the conflict zone
            // until we reach again the boundary of the conflict zone.
            // Traversing inside the conflict zone is faster (as compared
            // to outside) since it traverses a smaller number of tets.
            index_t cur_t = t1;
            index_t cur_f = t1ft2;
            index_t next_t = index_t(tet_adjacent(cur_t,cur_f));
            while(tet_is_marked_as_conflict(next_t)) {            
                geo_debug_assert(next_t != t1);
                cur_t = next_t;
                cur_f = get_facet_by_halfedge(cur_t,ev1,ev2);
                next_t = index_t(tet_adjacent(cur_t, cur_f));
            }
             
            //  At this point, cur_t is in conflict zone and
            // next_t is outside the conflict zone.
            index_t f12,f21;
            get_facets_by_halfedge(next_t, ev1, ev2, f12, f21);
            t2 = index_t(tet_adjacent(next_t,f21));
            signed_index_t v_neigh_opposite = tet_vertex(next_t,f12);
            t2ft1 = find_tet_vertex(t2, v_neigh_opposite);
            t2fborder = cur_f;
            
            //  Test whether the found neighboring tet was created
            //  (then return true) or is an old tet in conflict
            //  (then return false).
            return(t2 != cur_t);
        }

        StellateConflictStack S2_;
        
        

        void show_tet_adjacent(index_t t, index_t lf) const {
            signed_index_t adj = tet_adjacent(t, lf);
            if(adj != -1) {
                std::cerr << (tet_is_in_list(index_t(adj)) ? '*' : ' ');
            }
            std::cerr << adj;
            std::cerr << ' ';
        }

        
        void show_tet(index_t t) const {
            std::cerr << "tet"
                      << (tet_is_in_list(t) ? '*' : ' ')
                      << t
                      << ", v=["
                      << tet_vertex(t, 0)
                      << ' '
                      << tet_vertex(t, 1)
                      << ' '
                      << tet_vertex(t, 2)
                      << ' '
                      << tet_vertex(t, 3)
                      << "]  adj=[";
            show_tet_adjacent(t, 0);
            show_tet_adjacent(t, 1);
            show_tet_adjacent(t, 2);
            show_tet_adjacent(t, 3);
            std::cerr << "] ";
            
            for(index_t f = 0; f < 4; ++f) {
                std::cerr << 'f' << f << ':';
                for(index_t v = 0; v < 3; ++v) {
                    std::cerr << tet_vertex(t, tet_facet_vertex(f,v))
                              << ',';
                }
                std::cerr << ' ';
            }
            std::cerr << std::endl;
        }

    public:

        void check_combinatorics(bool verbose) const {
            if(verbose) {
                std::cerr << std::endl;
            }
            bool ok = true;
            std::vector<bool> v_has_tet(nb_vertices(), false);
            for(index_t t = 0; t < max_t(); ++t) {
                if(tet_is_free(t)) {
                    if(verbose) {
                        std::cerr << "-Deleted tet: ";
                        show_tet(t);
                    }
                } else {
                    if(verbose) {
                        std::cerr << "Checking tet: ";
                        show_tet(t);
                    }
                    for(index_t lf = 0; lf < 4; ++lf) {
                        if(tet_adjacent(t, lf) == -1) {
                            std::cerr << lf << ":Missing adjacent tet"
                                      << std::endl;
                            ok = false;
                        } else if(tet_adjacent(t, lf) == signed_index_t(t)) {
                            std::cerr << lf << ":Tet is adjacent to itself"
                                      << std::endl;
                            ok = false;
                        } else {
                            index_t t2 = index_t(tet_adjacent(t, lf));
                            bool found = false;
                            for(index_t lf2 = 0; lf2 < 4; ++lf2) {
                                if(tet_adjacent(t2, lf2) == signed_index_t(t)) {
                                    found = true;
                                }
                            }
                            if(!found) {
                                std::cerr
                                    << lf 
                                    << ":Adjacent link is not bidirectional"
                                    << std::endl;
                                ok = false;
                            }
                        }
                    }
                    index_t nb_infinite = 0;
                    for(index_t lv = 0; lv < 4; ++lv) {
                        if(tet_vertex(t, lv) == -1) {
                            ++nb_infinite;
                        }
                    }
                    if(nb_infinite > 1) {
                        ok = false;
                        std::cerr << "More than one infinite vertex"
                                  << std::endl;
                    }
                }
                for(index_t lv = 0; lv < 4; ++lv) {
                    signed_index_t v = tet_vertex(t, lv);
                    if(v >= 0) {
                        v_has_tet[periodic_vertex_real(index_t(v))] = true;
                    }
                }
            }

	    index_t nb_v = nb_vertices();
            for(index_t v = 0; v < nb_v; ++v) {
                if(!v_has_tet[v]) {
                    if(verbose) {
                        std::cerr << "Vertex " << v
                                  << " is isolated (duplicated ?)" << std::endl;
                    }
                }
            }
            geo_assert(ok);
            if(verbose) {
                std::cerr << std::endl;
            }
            std::cerr << std::endl << "Delaunay Combi OK" << std::endl;
        }


        void check_geometry(bool verbose) const {
            bool ok = true;
            for(index_t t = 0; t < max_t(); ++t) {
                if(!tet_is_free(t)) {
                    signed_index_t v0 = tet_vertex(t, 0);
                    signed_index_t v1 = tet_vertex(t, 1);
                    signed_index_t v2 = tet_vertex(t, 2);
                    signed_index_t v3 = tet_vertex(t, 3);
                    for(index_t v = 0; v < nb_vertices(); ++v) {
			vec4 p = lifted_vertex(v);
                        signed_index_t sv = signed_index_t(v);
                        if(sv == v0 || sv == v1 || sv == v2 || sv == v3) {
                            continue;
                        }
                        if(tet_is_in_conflict(t, v, p)) {
                            ok = false;
                            if(verbose) {
                                std::cerr << "Tet " << t <<
                                    " is in conflict with vertex " << v
                                          << std::endl;
                                
                                std::cerr << "  offending tet: ";
                                show_tet(t);
                            }
                        }
                    }
                }
            }
            geo_assert(ok);
            std::cerr << std::endl << "Delaunay Geo OK" << std::endl;
        }

    private:
        PeriodicDelaunay3d* master_;
	bool periodic_;
	double period_;
        index_t nb_vertices_;
        const double* vertices_;
	const double* weights_;
        index_t* reorder_;
        index_t dimension_;
        index_t max_t_;
        index_t max_used_t_;

        vector<signed_index_t>& cell_to_v_store_;
        vector<signed_index_t>& cell_to_cell_store_;
        vector<index_t>& cell_next_;
        vector<thread_index_t>& cell_thread_;
        
        index_t first_free_;
        index_t nb_free_;
        bool memory_overflow_;

        index_t v1_,v2_,v3_,v4_; // The first four vertices

	struct SFrame {
	    
	    SFrame() {
	    }
	    
	    SFrame(
		index_t t_in,
		index_t v_in,
		const vec4& p_in
	    ) :
		t(t_in),
		v(v_in),
		p(p_in) {
	    }
	    
	    SFrame(
		const SFrame& rhs
	    ):
		t(rhs.t),
		v(rhs.v),
		p(rhs.p) {
	    }

	    SFrame& operator=(const SFrame& rhs) {
		t = rhs.t;
		v = rhs.v;
		p = rhs.p;
		return *this;
	    }
	    
	    index_t t;
	    index_t v;
	    vec4 p;
	};
	
        vector<SFrame> S_;
        index_t nb_tets_to_create_;
        index_t t_boundary_; // index of a tet,facet on the bndry 
        index_t f_boundary_; // of the conflict zone.

        bool direction_;
        signed_index_t work_begin_;
        signed_index_t work_end_;
        index_t b_hint_;
        index_t e_hint_;
        bool finished_;

        //  Whenever acquire_tet() is unsuccessful, contains
        // the index of the thread that was interfering
        // (shifted to the left by 1 !!)
        thread_index_t interfering_thread_;

#ifdef GEO_DEBUG
        index_t nb_acquired_tets_;
#endif

        vector<index_t> tets_to_delete_;
        vector<index_t> tets_to_release_;

        index_t nb_rollbacks_;
        index_t nb_failed_locate_;

        pthread_cond_t cond_;
        pthread_mutex_t mutex_;

	vector<std::pair<index_t,index_t> > border_tet_2_periodic_vertex_;

	bool has_empty_cells_;
	
        static char tet_facet_vertex_[4][3];

        static char halfedge_facet_[4][4];

	Cavity cavity_;

	index_t nb_traversed_tets_;
    };


    char PeriodicDelaunay3dThread::halfedge_facet_[4][4] = {
        {4, 2, 3, 1},
        {3, 4, 0, 2},
        {1, 3, 4, 0},
        {2, 0, 1, 4}
    };

    // tet facet vertex is such that the tetrahedron
    // formed with:
    //  vertex lv
    //  tet_facet_vertex[lv][0]
    //  tet_facet_vertex[lv][1]
    //  tet_facet_vertex[lv][2]
    // has the same orientation as the original tetrahedron for
    // any vertex lv.

    char PeriodicDelaunay3dThread::tet_facet_vertex_[4][3] = {
        {1, 2, 3},
        {0, 3, 2},
        {3, 0, 1},
        {1, 0, 2}
    };


    

    
    
    PeriodicDelaunay3d::PeriodicDelaunay3d(
	bool periodic, double period
    ) :
	Delaunay(3),
	periodic_(periodic),
	period_(period),
	weights_(nullptr),
	update_periodic_v_to_cell_(false),
	has_empty_cells_(false),
	nb_reallocations_(0),
	convex_cell_exact_predicates_(true)
    {
	geo_cite_with_info(
	    "DBLP:journals/cj/Bowyer81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
        );
	geo_cite_with_info(
	    "journals/cj/Watson81",
	    "One of the two initial references to the algorithm, "
	    "discovered independently and simultaneously by Bowyer and Watson."
	);
	geo_cite_with_info(
	    "DBLP:conf/compgeom/AmentaCR03",
	    "Using spatial sorting has a dramatic impact on the performances."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/FunkeMN05",
	    "Initializing \\verb|locate()| with a non-exact version "
	    " (structural filtering) gains (a bit of) performance."
	);
	geo_cite_with_info(
	    "DBLP:journals/comgeo/BoissonnatDPTY02",
	    "The idea of traversing the cavity from inside "
	    " used in GEOGRAM is inspired by the implementation of "
	    " \\verb|Delaunay_triangulation_3| in CGAL."
	);
	geo_cite_with_info(
	    "DBLP:conf/imr/Si06",
	    "The triangulation data structure used in GEOGRAM is inspired "
	    "by Tetgen."
	);
	geo_cite_with_info(
	    "DBLP:journals/ijfcs/DevillersPT02",
	    "Analysis of the different versions of the line walk algorithm "
	    " used by \\verb|locate()|."
	);
	
        debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay");
        verbose_debug_mode_ = CmdLine::get_arg_bool("dbg:delaunay_verbose");
        debug_mode_ = (debug_mode_ || verbose_debug_mode_);
        benchmark_mode_ = CmdLine::get_arg_bool("dbg:delaunay_benchmark");
	nb_vertices_non_periodic_ = 0;
    }

    
    void PeriodicDelaunay3d::set_vertices(
        index_t nb_vertices, const double* vertices
    ) {
	#ifndef GARGANTUA
	{
	    Numeric::uint64 expected_max_index =
		Numeric::uint64(nb_vertices) * 7 * 4;
	    if(periodic_) {
		expected_max_index *= 2;
	    }
	    if(expected_max_index > Numeric::uint64(INT32_MAX)) {
		Logger::err("OTM") << "indices will overflow" << std::endl;
		Logger::err("OTM")
		    << "recompile with -DGARGANTUA " 
		    << "to activate 64bit indices" << std::endl;
		exit(0);
	    }
	}
	#endif

	if(periodic_) { 
	    PCK::set_SOS_mode(PCK::SOS_LEXICO);
	}
	
        Stopwatch* W = nullptr ;
        if(benchmark_mode_) {
            W = new Stopwatch("SpatialSort");
        }
	nb_vertices_non_periodic_ = nb_vertices;

        Delaunay::set_vertices(nb_vertices, vertices);
        // Reorder the points
        if(do_reorder_) {
            compute_BRIO_order(
                nb_vertices, vertex_ptr(0), reorder_,
		3, dimension(),
                64, 0.125,
                &levels_
            );        
        } else {
            reorder_.resize(nb_vertices);
            for(index_t i = 0; i < nb_vertices; ++i) {
                reorder_[i] = i;
            }
            geo_debug_assert(levels_[0] == 0);
            geo_debug_assert(levels_[levels_.size()-1] == nb_vertices);
        }
	delete W;
    }

    void PeriodicDelaunay3d::set_weights(const double* weights) {
	weights_ = weights;	
    }
    
    void PeriodicDelaunay3d::compute() {
	
	has_empty_cells_ = false;
	
	if(periodic_) {
	    reorder_.resize(nb_vertices_non_periodic_);
	}
	
        Stopwatch* W = nullptr ;
        if(benchmark_mode_) {
            W = new Stopwatch("DelInternal");
        }

        index_t expected_tetra = nb_vertices() * 7;
    
        // Allocate the tetrahedra
        cell_to_v_store_.assign(expected_tetra * 4,-1);
        cell_to_cell_store_.assign(expected_tetra * 4,-1);
        cell_next_.assign(expected_tetra,index_t(-1));
        cell_thread_.assign(expected_tetra,thread_index_t(-1));


        // Create the threads
        index_t nb_threads = Process::maximum_concurrent_threads();
        index_t pool_size = expected_tetra / nb_threads;
        index_t pool_begin = 0;
        threads_.clear();
        for(index_t t=0; t<nb_threads; ++t) {
            index_t pool_end = 
                (t == nb_threads - 1) ? expected_tetra : pool_begin + pool_size;
            threads_.push_back(
                new PeriodicDelaunay3dThread(this, pool_begin, pool_end)
            );
            pool_begin = pool_end;
        }


        // Create first tetrahedron and triangulate first set of points 
        // in sequential mode.


        index_t lvl = 1;
        while(lvl < (levels_.size() - 1) && levels_[lvl] < 1000) {
            ++lvl;
        }
	
        if(benchmark_mode_) {
            Logger::out("PDEL")
                << "Using " << levels_.size()-1 << " levels" << std::endl;
            Logger::out("PDEL") 
                << "Levels 0 - " << lvl-1 
                << ": bootstraping with first levels in sequential mode"
                << std::endl;
        }
        PeriodicDelaunay3dThread* thread0 = thread(0);
        thread0->create_first_tetrahedron();
	thread0->set_work(levels_[0], levels_[lvl]);
        thread0->run();

	if(thread0->has_empty_cells()) {
	    has_empty_cells_ = true;
	    return;
	}
	
        index_t first_lvl = lvl;

        // Insert points in all BRIO levels
        for(; lvl<levels_.size()-1; ++lvl) {

            if(benchmark_mode_) {
                Logger::out("PDEL") << "Level " 
                                    << lvl << " : start" << std::endl;
            }

            index_t lvl_b = levels_[lvl];
            index_t lvl_e = levels_[lvl+1];
            index_t work_size = (lvl_e - lvl_b)/index_t(threads_.size());

            // Initialize threads
            index_t b = lvl_b;
            for(index_t t=0; t<threads_.size(); ++t) {
                index_t e = t == threads_.size()-1 ? lvl_e : b+work_size;
                
                // Copy the indices of the first created tetrahedron
                // and the maximum valid tetrahedron index max_t_
                if(lvl == first_lvl && t!=0) {
                    thread(t)->initialize_from(thread0);
                }
                thread(t)->set_work(b,e);
                b = e;
            }
            Process::run_threads(threads_);

	    for(index_t t=0; t<this->nb_threads(); ++t) {
		if(thread(t)->has_empty_cells()) {
		    has_empty_cells_ = true;
		    return;
		}
	    }
        }


        if(benchmark_mode_) {
            index_t tot_rollbacks = 0 ;
            index_t tot_failed_locate = 0 ;
            for(index_t t=0; t<threads_.size(); ++t) {
                Logger::out("PDEL") 
                    << "thread " << t << " : " 
                    << thread(t)->nb_rollbacks() << " rollbacks  "
                    << thread(t)->nb_failed_locate() << " failed locate"
                    << std::endl;
                tot_rollbacks += thread(t)->nb_rollbacks();
                tot_failed_locate += thread(t)->nb_failed_locate();
            }
            Logger::out("PDEL") << "------------------" << std::endl;
            Logger::out("PDEL") << "total: " 
                                << tot_rollbacks << " rollbacks  "
                                << tot_failed_locate << " failed locate"
                                << std::endl;
        }

        // Run threads sequentialy, to insert missing points if
        // memory overflow was encountered (in sequential mode,
        // dynamic memory growing works)

        index_t nb_sequential_points = 0;
        for(index_t t=0; t<threads_.size(); ++t) {
            PeriodicDelaunay3dThread* t1 = thread(t);
            nb_sequential_points += t1->work_size();
            if(t != 0) {
                // We need to copy max_t_ from previous thread, 
                // since the memory pool may have grown.
                PeriodicDelaunay3dThread* t2 = thread(t-1);
                t1->initialize_from(t2);
            }
            t1->run();
        }

        //  If some tetrahedra were created in sequential mode, then
        // the maximum valid tetrahedron index was increased by all
        // the threads in increasing number, so we copy it from the
        // last thread into thread0 since we use thread0 afterwards
        // to do the "compaction" afterwards.
        
        if(nb_sequential_points != 0) {
            PeriodicDelaunay3dThread* t0 = thread(0);
	    PeriodicDelaunay3dThread* tn = thread(
		this->nb_threads()-1
	    );
            t0->initialize_from(tn);
        }
        
	if(periodic_) {
	    handle_periodic_boundaries();
	}

	if(has_empty_cells_) {
	    return;
	}
	
        if(benchmark_mode_) {
            if(nb_sequential_points != 0) {
                Logger::out("PDEL") << "Local thread memory overflow occurred:"
                                    << std::endl;
                Logger::out("PDEL") << nb_sequential_points
                                    << " points inserted in sequential mode"
                                    << std::endl;
            } else {
                Logger::out("PDEL") 
                    << "All the points were inserted in parallel mode"
                    << std::endl;
            }
        }

        if(benchmark_mode_) {
            Logger::out("DelInternal2") << "Core insertion algo:"
                                       << W->elapsed_time() 
                                       << std::endl;
        }
        delete W;

        if(debug_mode_) {
            for(index_t i=0; i<threads_.size(); ++i) {
                std::cerr << i << " : " <<
                    static_cast<PeriodicDelaunay3dThread*>(threads_[i].get())
                    ->max_t() << std::endl;
            }
            
            thread0->check_combinatorics(verbose_debug_mode_);
            thread0->check_geometry(verbose_debug_mode_);
        }

	index_t nb_tets = compress();

        set_arrays(
            nb_tets,
            cell_to_v_store_.data(),
            cell_to_cell_store_.data()
        );

	// We need v_to_cell even if CICL is not
	// stored. 
	if(!stores_cicl()) {
	    update_v_to_cell();
	}
	
	if(periodic_) {
#ifdef GEO_DEBUG	    
	    FOR(v, nb_vertices_non_periodic_) {
		index_t t = index_t(v_to_cell_[v]);
		geo_assert(t == index_t(-1) || t < nb_tets);
	    }
#endif	    
	}
    }

    index_t PeriodicDelaunay3d::compress(bool shrink) {
        //   Compress cell_to_v_store_ and cell_to_cell_store_
        // (remove free and virtual tetrahedra).
        //   Since cell_next_ is not used at this point,
        // we reuse it for storing the conversion array that
        // maps old tet indices to new tet indices
        // Note: tet_is_real() uses the previous value of 
        // cell_next(), but we are processing indices
        // in increasing order and since old2new[t] is always
        // smaller or equal to t, we never overwrite a value
        // before needing it.


        PeriodicDelaunay3dThread* thread0 = thread(0);
        vector<index_t>& old2new = cell_next_;
        index_t nb_tets = 0;
        index_t nb_tets_to_delete = 0;

        {
            for(index_t t = 0; t < thread0->max_t(); ++t) {
                if(
                    (keep_infinite_ && !thread0->tet_is_free(t)) ||
		    (periodic_ && thread0->tet_is_real_non_periodic(t)) ||
                    (!periodic_ && thread0->tet_is_real(t))
                ) {
                    if(t != nb_tets) {
                        Memory::copy(
                            &cell_to_v_store_[nb_tets * 4],
                            &cell_to_v_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                        Memory::copy(
                            &cell_to_cell_store_[nb_tets * 4],
                            &cell_to_cell_store_[t * 4],
                            4 * sizeof(signed_index_t)
                        );
                    }
                    old2new[t] = nb_tets;
                    ++nb_tets;
                } else {
                    old2new[t] = index_t(-1);
                    ++nb_tets_to_delete;
                }
            }

	    if(shrink) {
		cell_to_v_store_.resize(4 * nb_tets);
		cell_to_cell_store_.resize(4 * nb_tets);
	    }
	    
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                // Note: t can be equal to -1 when a real tet is
                // adjacent to a virtual one (and this is how the
                // rest of Vorpaline expects to see tets on the
                // border).
                geo_debug_assert(!(keep_infinite_ && t < 0));
                cell_to_cell_store_[i] = t;
            }
        }

        // In "keep_infinite" mode, we reorder the cells in such
        // a way that finite cells have indices [0..nb_finite_cells_-1]
        // and infinite cells have indices [nb_finite_cells_ .. nb_cells_-1]
        
        if(keep_infinite_) {
            nb_finite_cells_ = 0;
            index_t finite_ptr = 0;
            index_t infinite_ptr = nb_tets - 1;
            for(;;) {
                while(thread0->tet_is_finite(finite_ptr)) {
                    old2new[finite_ptr] = finite_ptr;
                    ++finite_ptr;
                    ++nb_finite_cells_;
                }
                while(!thread0->tet_is_finite(infinite_ptr)) {
                    old2new[infinite_ptr] = infinite_ptr;
                    --infinite_ptr;
                }
                if(finite_ptr > infinite_ptr) {
                    break;
                }
                old2new[finite_ptr] = infinite_ptr;
                old2new[infinite_ptr] = finite_ptr;
                ++nb_finite_cells_;
                for(index_t lf=0; lf<4; ++lf) {
                    std::swap(
                        cell_to_cell_store_[4*finite_ptr + lf],
                        cell_to_cell_store_[4*infinite_ptr + lf]
                    );
                }
                for(index_t lv=0; lv<4; ++lv) {
                    std::swap(
                        cell_to_v_store_[4*finite_ptr + lv],
                        cell_to_v_store_[4*infinite_ptr + lv]
                    );
                }
                ++finite_ptr;
                --infinite_ptr;
            }
            for(index_t i = 0; i < 4 * nb_tets; ++i) {
                signed_index_t t = cell_to_cell_store_[i];
                geo_debug_assert(t >= 0);
                t = signed_index_t(old2new[t]);
                geo_debug_assert(t >= 0);
                cell_to_cell_store_[i] = t;
            }
        }
        
        
        if(benchmark_mode_) {
	    Logger::out("DelCompress") 
		<< "max tets " << thread0->max_t()
		<< std::endl;
	    
	    Logger::out("DelCompress") 
		<< "Final number of tets " << nb_tets
		<< std::endl;
            if(keep_infinite_) {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list)" << std::endl;
            } else {
                Logger::out("DelCompress") 
                    << "Removed " << nb_tets_to_delete 
                    << " tets (free list and infinite)" << std::endl;
            }
        }

	for(index_t t=0; t<nb_tets; ++t) {
	    cell_next_[t] = PeriodicDelaunay3dThread::NOT_IN_LIST;
	}

	if(periodic_) {
	    FOR(i, 4*nb_tets) {
		if(cell_to_cell_store_[i] >= int(nb_tets)) {
		    cell_to_cell_store_[i] = -1;
		}
	    }
	    FOR(i, 4*nb_tets) {
		geo_debug_assert(cell_to_v_store_[i] != -1);
	    }
	}
	return nb_tets;
    }
    
    index_t PeriodicDelaunay3d::nearest_vertex(const double* p) const {
        // TODO
        return Delaunay::nearest_vertex(p);
    }

    void PeriodicDelaunay3d::set_BRIO_levels(const vector<index_t>& levels) {
        levels_ = levels;
    }

    void PeriodicDelaunay3d::update_v_to_cell() {
        geo_assert(!is_locked_);  // Not thread-safe
        is_locked_ = true;

	// Note: if keeps_infinite is set, then infinite vertex
	// tet chaining is at t2v_[nb_vertices].

	// Create periodic_v_to_cell_ structure in compressed row
	// storage format.
	if(update_periodic_v_to_cell_) {
	    periodic_v_to_cell_rowptr_.resize(nb_vertices_non_periodic_ + 1);
	    periodic_v_to_cell_rowptr_[0] = 0;
	    index_t cur = 0;
	    for(index_t v=0; v<nb_vertices_non_periodic_; ++v) {
		cur += pop_count(vertex_instances_[v])-1;
		periodic_v_to_cell_rowptr_[v+1] = cur;
	    }
	    periodic_v_to_cell_data_.assign(cur, index_t(-1));
	}
	
	if(keeps_infinite()) {
	    geo_assert(!periodic_);
	    v_to_cell_.assign(nb_vertices()+1, -1);
	    for(index_t c = 0; c < nb_cells(); c++) {
		for(index_t lv = 0; lv < 4; lv++) {
		    signed_index_t v = cell_vertex(c, lv);
		    if(v == -1) {
			v = signed_index_t(nb_vertices());
		    }
		    v_to_cell_[v] = signed_index_t(c);
		}
	    }
	} else {
	    v_to_cell_.assign(nb_vertices(), -1);	    
	    for(index_t c = 0; c < nb_cells(); c++) {
		for(index_t lv = 0; lv < 4; lv++) {
		    index_t v = index_t(cell_vertex(c, lv));
		    if(v < nb_vertices_non_periodic_) {
			v_to_cell_[v] = signed_index_t(c);
		    } else if(
			update_periodic_v_to_cell_ &&
			v != index_t(-1) && v != index_t(-2)
		    ) {
			index_t v_real = periodic_vertex_real(v);
			index_t v_instance = periodic_vertex_instance(v);

			geo_debug_assert((vertex_instances_[v_real] & (1u << v_instance))!=0);
			
			index_t slot = pop_count(
			       vertex_instances_[v_real] & ((1u << v_instance)-1)
			) - 1;

			periodic_v_to_cell_data_[
			    periodic_v_to_cell_rowptr_[v_real] + slot
			] = c;
			
			// periodic_v_to_cell_[v] = c;
		    }
		}
	    }
	}

        is_locked_ = false;
    }

    void PeriodicDelaunay3d::update_cicl() {
	// Note: updates CICL information only for the
	// corners that correspond to real vertices
	// (anyway, in the API we will be always
	// starting from a real vertex !)

        geo_assert(!is_locked_);  // Not thread-safe
        is_locked_ = true;
        cicl_.resize(4 * nb_cells());

	for(index_t v = 0; v < nb_vertices_non_periodic_; ++v) {
	    signed_index_t t = v_to_cell_[v];
	    if(t != -1) {
		index_t lv = index(index_t(t), signed_index_t(v));
		set_next_around_vertex(index_t(t), lv, index_t(t));
	    }
	}
	
	if(keeps_infinite()) {

	    {
		// Process the infinite vertex at index nb_vertices().
		signed_index_t t = v_to_cell_[nb_vertices()];
		if(t != -1) {
		    index_t lv = index(index_t(t), -1);
		    set_next_around_vertex(index_t(t), lv, index_t(t));
		}
	    }

	    for(index_t t = 0; t < nb_cells(); ++t) {
		for(index_t lv = 0; lv < 4; ++lv) {
		    signed_index_t v = cell_vertex(t, lv);
		    index_t vv = (v == -1) ? nb_vertices() : index_t(v);
		    if(v_to_cell_[vv] != signed_index_t(t)) {
			index_t t1 = index_t(v_to_cell_[vv]);
			index_t lv1 = index(t1, signed_index_t(v));
			index_t t2 = index_t(next_around_vertex(t1, lv1));
			set_next_around_vertex(t1, lv1, t);
			set_next_around_vertex(t, lv, t2);
		    }
		}
	    }
	    
	    
	} else {
	    for(index_t t = 0; t < nb_cells(); ++t) {
		for(index_t lv = 0; lv < 4; ++lv) {
		    index_t v = index_t(cell_vertex(t, lv));
		    if(
			v < nb_vertices_non_periodic_ &&
			v_to_cell_[v] != signed_index_t(t)
		    ) {
			index_t t1 = index_t(v_to_cell_[v]);
			index_t lv1 = index(t1, signed_index_t(v));
			index_t t2 = index_t(next_around_vertex(t1, lv1));
			set_next_around_vertex(t1, lv1, t);
			set_next_around_vertex(t, lv, t2);
		    }
		}
	    }
	}
	
        is_locked_ = false;
    }

    void PeriodicDelaunay3d::get_incident_tets(index_t v, IncidentTetrahedra& W) const {

	geo_debug_assert(
	    periodic_ || v < nb_vertices_non_periodic_
	);

	W.clear_incident_tets();

	index_t t = index_t(-1);
	if(v < nb_vertices_non_periodic_) {
	    t = index_t(v_to_cell_[v]);
	} else {
	    index_t v_real = periodic_vertex_real(v);
	    index_t v_instance = periodic_vertex_instance(v);
	    
	    geo_debug_assert((vertex_instances_[v_real] & (1u << v_instance))!=0);

	    index_t slot = pop_count(
		vertex_instances_[v_real] & ((1u << v_instance)-1)
	    ) - 1;

	    t = periodic_v_to_cell_data_[
		periodic_v_to_cell_rowptr_[v_real] + slot
	    ];
	}
	
	// Can happen: empty power cell.
	if(t == index_t(-1)) {
	    return;
	}

	// TODO: different version if CICL is stored ?
	{
	    W.add_incident_tet(t);
	    W.S.push(t);
	    while(!W.S.empty()) {
		t = W.S.top();
		W.S.pop();
		const signed_index_t* T = &(cell_to_v_store_[4 * t]);
		index_t lv = PeriodicDelaunay3dThread::find_4(T,signed_index_t(v));
		index_t neigh = index_t(cell_to_cell_store_[4*t + (lv + 1)%4]);
		if(neigh != index_t(-1) && !W.has_incident_tet(neigh)) {
		    W.add_incident_tet(neigh);
		    W.S.push(neigh);
		}
		neigh = index_t(cell_to_cell_store_[4*t + (lv + 2)%4]);
		if(neigh != index_t(-1) && !W.has_incident_tet(neigh)) {
		    W.add_incident_tet(neigh);
		    W.S.push(neigh);
		}
		neigh = index_t(cell_to_cell_store_[4*t + (lv + 3)%4]);
		if(neigh != index_t(-1) && !W.has_incident_tet(neigh)) {
		    W.add_incident_tet(neigh);
		    W.S.push(neigh);
		}
	    }
	}
    }

    inline double dist2(const double* p, const double* q) {
	return
	    geo_sqr(p[0]-q[0]) +
	    geo_sqr(p[1]-q[1]) +
	    geo_sqr(p[2]-q[2]) ; 
    }

    void PeriodicDelaunay3d::copy_Laguerre_cell_from_Delaunay(
	GEO::index_t i,
	ConvexCell& C,
	IncidentTetrahedra& W
    ) const {
	// Create global vertex indices if not present.
	C.create_vglobal();
	C.clear();
	
	// Create the vertex at infinity.
	C.create_vertex(vec4(0.0, 0.0, 0.0, 0.0), index_t(-1));
	
	GEO::vec3 Pi = vertex(i);
	double wi = weight(i);
	double Pi_len2 = Pi[0]*Pi[0] + Pi[1]*Pi[1] + Pi[2]*Pi[2];

	if(stores_cicl()) {
	    // Get neighbors and initialize cell from the tetrahedra in
	    // 1-ring neighborhood of vertex.
	    GEO::index_t t = GEO::index_t(vertex_cell(i));
	    // Special case: Laguerre cell is empty (vertex has
	    // no incident tet).
	    if(t == (GEO::index_t)(-1)) {
		return;
	    }
	    do {
		GEO::index_t f = copy_Laguerre_cell_facet_from_Delaunay(
		    i, Pi, wi, Pi_len2, t, C, W
		);
		t = GEO::index_t(next_around_vertex(t,f));
	    } while(t != GEO::index_t(vertex_cell(i)));
	} else {
	    get_incident_tets(i,W);
	    for(index_t t: W) {
		copy_Laguerre_cell_facet_from_Delaunay(
		    i, Pi, wi, Pi_len2, t, C, W
	        );
	    }
	}
    }

    
    GEO::index_t PeriodicDelaunay3d::copy_Laguerre_cell_facet_from_Delaunay(
	GEO::index_t i,
	const GEO::vec3& Pi,
	double wi,
	double Pi_len2,
	GEO::index_t t,
	ConvexCell& C,
	IncidentTetrahedra& W
    ) const {
	geo_argused(W);
	
	// Local tet vertex indices from facet
	// and vertex in facet indices.
	static GEO::index_t fv[4][3] = {
	    {1,3,2},
	    {0,2,3},
	    {3,1,0},
	    {0,1,2}
	};
	
	GEO::index_t f = index(t,GEO::signed_index_t(i));
	GEO::index_t jkl[3];  // Global index (in Delaunay) of triangle vertices
	VBW::index_t l_jkl[3];// Local index (in C) of triangle vertices

	
	// Find or create the three vertices of the facet.
	for(int lfv=0; lfv<3; ++lfv) {

	    jkl[lfv] = GEO::index_t(cell_vertex(t, fv[f][lfv]));
	    l_jkl[lfv] = VBW::index_t(-1);
	    
	    // Vertex already created in C (note:
	    // also works for vertex at infinity)
	    for(VBW::index_t u=0; u<C.nb_v(); ++u) {
		if(C.v_global_index(u) == jkl[lfv]) {
		    l_jkl[lfv] = VBW::index_t(u);
		    break;
		}
	    }

	    // vertex not found, create vertex in C
	    if(l_jkl[lfv] == VBW::index_t(-1)) {
		l_jkl[lfv] = C.nb_v();
		GEO::vec3 Pj = vertex(jkl[lfv]);
		double wj = weight(jkl[lfv]);
		double a = 2.0 * (Pi[0] - Pj[0]);
		double b = 2.0 * (Pi[1] - Pj[1]);
		double c = 2.0 * (Pi[2] - Pj[2]);
		double d = ( Pj[0]*Pj[0] +
			     Pj[1]*Pj[1] +
			     Pj[2]*Pj[2] ) - Pi_len2 + wi - wj;
		C.create_vertex(vec4(a,b,c,d), jkl[lfv]);
	    }
	}

	// Note: need to invert orientation (TODO: check why, I probably
	//  used opposite conventions in Delaunay and VBW, stupid me !!)
	C.create_triangle(l_jkl[2], l_jkl[1], l_jkl[0]);

	return f;
    }

    

    void PeriodicDelaunay3d::insert_vertices(index_t b, index_t e) {
	nb_vertices_ = reorder_.size();
	
	PeriodicDelaunay3dThread* thread0 = thread(0);

	Hilbert_sort_periodic(
	    // total nb of possible periodic vertices
	    nb_vertices_non_periodic_ * 27, 
	    vertex_ptr(0),
	    reorder_,
	    3, dimension(), 
	    reorder_.begin() + long(b),
	    reorder_.begin() + long(e),
	    period_
	);
	
	if(benchmark_mode_) {
	    Logger::out("Periodic") << "Inserting "	<< (e-b)
				    << " additional vertices" << std::endl;
	}

#ifdef GEO_DEBUG	    
	for(index_t i=b; i+1<e; ++i) {
	    geo_debug_assert(reorder_[i] != reorder_[i+1]);
	}
#endif
	
	nb_reallocations_ = 0;
		
	index_t expected_tetra = reorder_.size() * 7;
	cell_to_v_store_.reserve(expected_tetra * 4);
	cell_to_cell_store_.reserve(expected_tetra * 4);
	cell_next_.reserve(expected_tetra);
	cell_thread_.reserve(expected_tetra);

	index_t total_nb_traversed_tets = 0;

	index_t hint = index_t(-1);
	for(index_t i = b; i<e; ++i) {
	    thread0->insert(reorder_[i],hint);
	    total_nb_traversed_tets += thread0->nb_traversed_tets();
	    if(hint == index_t(-1)) {
		has_empty_cells_ = true;
		return;
	    }
	}

	if(benchmark_mode_) {
	    if(nb_reallocations_ != 0) {
		Logger::out("Periodic") << nb_reallocations_
					<< " reallocation(s)" << std::endl;
	    }
	    Logger::out("Periodic")
		<< double(total_nb_traversed_tets) / double(e-b)
		<< " avg. traversed tet per insertion." << std::endl;
	}
	
	set_arrays(
	    thread0->max_t(),
	    cell_to_v_store_.data(),
	    cell_to_cell_store_.data()
	);
    }

    index_t PeriodicDelaunay3d::get_periodic_vertex_instances_to_create(
	index_t v,
	ConvexCell& C,
	bool use_instance[27],
	bool& cell_is_on_boundary,
	bool& cell_is_outside_cube,
	IncidentTetrahedra& W
    ) {	
	// Integer translations associated with the six plane equations
	// Note: indexing matches code below (order of the clipping
	// operations).
	static int T[6][3]= {
	    { 1, 0, 0},
	    {-1, 0, 0},
	    { 0, 1, 0},
	    { 0,-1, 0},
	    { 0, 0, 1},
	    { 0, 0,-1}
	};

	copy_Laguerre_cell_from_Delaunay(v, C, W);
	geo_assert(!C.empty());
	    
	// Determine the periodic instances of the vertex to be created
	// ************************************************************
	//   - Find all the inresected boundary faces
	//   - The instances to create correspond to all the possible
	//     sums of translation vectors associated with the
	//     intersected boundary faces
	
	FOR(i,27) {
	    use_instance[i] = false;
	}
	use_instance[0] = true;

	index_t cube_offset = C.nb_v();
	C.clip_by_plane(vec4( 1.0, 0.0, 0.0,  0.0));
	C.clip_by_plane(vec4(-1.0, 0.0, 0.0,  period_));
	C.clip_by_plane(vec4( 0.0, 1.0, 0.0,  0.0));
	C.clip_by_plane(vec4( 0.0,-1.0, 0.0,  period_));	
	C.clip_by_plane(vec4( 0.0, 0.0, 1.0,  0.0));
	C.clip_by_plane(vec4( 0.0, 0.0,-1.0,  period_));

	cell_is_outside_cube = false;
	cell_is_on_boundary = false;
	
	if(C.empty()) {
	    // Special case: cell is completely outside the cube.	    
	    cell_is_outside_cube = true;
	    copy_Laguerre_cell_from_Delaunay(v, C, W);
	    // Clip the cell with the 3x3x3 (rubic's) cube that
	    // surrounds the cube. Not only this avoids generating
	    // some unnecessary virtual vertices, but also, without
	    // it, it would generate neighborhoods with virtual vertices
	    // coordinates that differ by more than twice the period.
	    C.clip_by_plane(vec4( 1.0, 0.0, 0.0,  period_));
	    C.clip_by_plane(vec4(-1.0, 0.0, 0.0,  2.0*period_));
	    C.clip_by_plane(vec4( 0.0, 1.0, 0.0,  period_));
	    C.clip_by_plane(vec4( 0.0,-1.0, 0.0,  2.0*period_));	
	    C.clip_by_plane(vec4( 0.0, 0.0, 1.0,  period_));
	    C.clip_by_plane(vec4( 0.0, 0.0,-1.0,  2.0*period_));

	    // Normally we cannot have an empty cell, empty cells were
	    // detected before.
	    geo_assert(!C.empty());
	    
	    // Now detect the bounds of the sub-(rubic's) cube overlapped
	    // by the cell.
	    int TXmin = 0, TXmax = 0,
		TYmin = 0, TYmax = 0,
		TZmin = 0, TZmax = 0;
	    if(C.cell_has_conflict(vec4( 1.0, 0.0, 0.0,  0.0))) {
		TXmin = -1;
	    }
	    if(C.cell_has_conflict(vec4(-1.0, 0.0, 0.0,  period_))) {
		TXmax = 1;
	    }
	    if(C.cell_has_conflict(vec4( 0.0, 1.0, 0.0,  0.0))) {
		TYmin = -1;
	    }
	    if(C.cell_has_conflict(vec4( 0.0,-1.0, 0.0,  period_))) {
		TYmax = 1;
	    }
	    if(C.cell_has_conflict(vec4( 0.0, 0.0, 1.0,  0.0))) {
		TZmin = -1;
	    } 
	    if(C.cell_has_conflict(vec4( 0.0, 0.0,-1.0,  period_))) {
		TZmax = 1;
	    } 
	    for(int TX = TXmin; TX <= TXmax; ++TX) {
		for(int TY = TYmin; TY <= TYmax; ++TY) {
		    for(int TZ = TZmin; TZ <= TZmax; ++TZ) {
			use_instance[T_to_instance(-TX,-TY,-TZ)] = true;
		    }
		}
	    }
	} else {
	    // Back to the normal case, C contains the Laguerre cell clipped
	    // by the cube.
	    //   - Find all the intersected boundary faces
	    //   - The instances to create correspond to all the possible
	    //     sums of translation vectors associated with the
	    //     intersected boundary faces
	    
	    // Traverse all the Voronoi vertices of the face
	    for(
		VBW::ushort t = C.first_triangle();
		t!=VBW::END_OF_LIST; t=C.next_triangle(t)
	    ) {
		// The three (integer) translations associated with the
		// three faces on which the Voronoi vertex resides.
		int VXLAT[3][3];
		bool vertex_on_boundary = false;
		for(index_t lv=0; lv<3; ++lv) {
		    index_t pp = C.triangle_v_local_index(t,lv);
		    if(pp < cube_offset) {
			// Not a boundary face -> translation is zero.
			VXLAT[lv][0] = 0;
			VXLAT[lv][1] = 0;
			VXLAT[lv][2] = 0;
		    } else {
			// Boundary face -> there is a translation
			VXLAT[lv][0] = T[pp - cube_offset][0];
			VXLAT[lv][1] = T[pp - cube_offset][1];
			VXLAT[lv][2] = T[pp - cube_offset][2];
			vertex_on_boundary = true;
		    }
		}
		// If the vertex is on the boundary, mark all the instances
		// obtained by applying any combination of the (up to 3)
		// translations associated with the (up to 3)
		// boundary facets on which the vertex resides.
		if(vertex_on_boundary) {
		    cell_is_on_boundary = true;
		    for(int U=0; U<2; ++U) {
			for(int V=0; V<2; ++V) {
			    for(int W=0; W<2; ++W) {
				int Tx = U*VXLAT[0][0] + V*VXLAT[1][0] + W*VXLAT[2][0];
				int Ty = U*VXLAT[0][1] + V*VXLAT[1][1] + W*VXLAT[2][1];
				int Tz = U*VXLAT[0][2] + V*VXLAT[1][2] + W*VXLAT[2][2];
				use_instance[T_to_instance(Tx,Ty,Tz)] = true;
			    }
			}
		    }
		}
	    }
	}
	index_t result = 0;
	for(index_t i=1; i<27; ++i) {
	    result += (use_instance[i] ? 1 : 0);
	}
	return result;
    }
    
    void PeriodicDelaunay3d::handle_periodic_boundaries() {

	// Update pointers so that queries function will work (even in our
	// "transient state").
        PeriodicDelaunay3dThread* thread0 = thread(0);

        set_arrays(
	    thread0->max_t(),
            cell_to_v_store_.data(),
            cell_to_cell_store_.data()
        );

	update_v_to_cell();
	if(stores_cicl()) {
	    update_cicl();
	}
	
	for(index_t v=0; v<nb_vertices_non_periodic_; ++v) {
	    if(v_to_cell_[v] == -1) {
		has_empty_cells_ = true;
		return;
	    }
	}
	
	// -----------------------------------------------------------
	// Phase I: find the cells that intersect the boundaries, and
	// create periodic vertices for each possible translation.
	// -----------------------------------------------------------

	// Indicates for each real vertex the instances it has.
	// Each bit of vertex_instances_[v] indicates which instance
	// is used.
	vertex_instances_.assign(nb_vertices_non_periodic_,1);
	
	index_t nb_cells_on_boundary = 0;
	index_t nb_cells_outside_cube = 0;

	Process::spinlock lock = GEOGRAM_SPINLOCK_INIT;

	parallel_for_slice(
	    0, nb_vertices_non_periodic_,		     
	    [this,&lock,&nb_cells_on_boundary,&nb_cells_outside_cube](
		index_t from, index_t to
	    ) {
		ConvexCell C;
		C.use_exact_predicates(convex_cell_exact_predicates_);
		IncidentTetrahedra W;

	      for(index_t v=from; v<to; ++v) {
		  bool use_instance[27];
		  bool cell_is_on_boundary = false;
		  bool cell_is_outside_cube = false;
		  
		  // Determines the periodic vertices to create, that is,
		  // whenever the cell of the current vertex has an intersection
		  // with one of the 27 cubes, an instance needs to be created there.
		  index_t nb_instances = get_periodic_vertex_instances_to_create(
		      v, C, use_instance,
		      cell_is_on_boundary, cell_is_outside_cube,
		      W
		  );


		  Process::acquire_spinlock(lock);
		  
		  if(cell_is_on_boundary) {
		      ++nb_cells_on_boundary;
		  }

		  if(cell_is_outside_cube) {
		      ++nb_cells_outside_cube;
		  }
	    
		  // Append the new periodic vertices in the list of vertices.
		  // (we put them in the reorder_ vector that is always used
		  //  to do the insertions).
		  if(nb_instances > 0) {
		      for(index_t instance=1; instance<27; ++instance) {
			  if(use_instance[instance]) {
			      vertex_instances_[v] |= (1u << instance);
			      reorder_.push_back(make_periodic_vertex(v,instance));
			  }
		      }
		  }
		  
		  Process::release_spinlock(lock);		
	      }
	   }
	);


	if(benchmark_mode_) {
	    Logger::out("Periodic") << "Nb cells on boundary = "
				    << nb_cells_on_boundary
				    << std::endl;

	    Logger::out("Periodic") << "Nb cells outside cube = "
				    << nb_cells_outside_cube
				    << std::endl;
	}

	insert_vertices(nb_vertices_non_periodic_, reorder_.size());

	// This flag to tell update_v_to_cell() to
	// also update the table for periodic (virtual)
	// vertices (the periodic_v_to_cell_ table).
	update_periodic_v_to_cell_ = true;
	update_v_to_cell();
	update_periodic_v_to_cell_ = false;	    	    
	if(stores_cicl()) {
	    update_cicl();
	}

	index_t nb_vertices_phase_I = reorder_.size();

	// -----------------------------------------------------------	
	// Phase II: Insert the real neighbors of the virtual vertices,
	//  back-translated to the original position.
	// -----------------------------------------------------------		

	{
	    IncidentTetrahedra W;

	    // vertex_instances_ is used to implement v2t_ for virtual
	    // vertices, we cannot modify it here, so we create a copy
	    // that we will modify.
	    vector<index_t> vertex_instances = vertex_instances_;
	    for(index_t v=0; v<nb_vertices_non_periodic_; ++v) {
		
		for(index_t instance=1; instance<27; ++instance) {
		    int vTx = translation[instance][0];
		    int vTy = translation[instance][1];
		    int vTz = translation[instance][2];
		    
		    if((vertex_instances_[v] & (1u << instance)) != 0) {
			index_t vp = make_periodic_vertex(v, instance);
			get_incident_tets(vp, W);
			for(index_t t: W) {
			    FOR(lv, 4) {
				index_t wp = index_t(cell_vertex(t,lv));
				if(wp != vp && wp != index_t(-1)) {
				    index_t w = periodic_vertex_real(wp);
				    index_t w_instance = periodic_vertex_instance(wp);
				    int wTx = translation[w_instance][0];
				    int wTy = translation[w_instance][1];
				    int wTz = translation[w_instance][2];
				    int Tx = wTx - vTx;
				    int Ty = wTy - vTy;
				    int Tz = wTz - vTz;
				    if(
					Tx < -1 || Tx > 1 ||
					Ty < -1 || Ty > 1 ||
					Tz < -1 || Tz > 1
				    ) {
					std::cerr << "FATAL ERROR: large displacement !!"
						  << std::endl;
					geo_assert_not_reached;
				    } else {
					index_t w_new_instance = T_to_instance(Tx, Ty, Tz);
					if((vertex_instances[w] & (1u << w_new_instance)) == 0) {
					    vertex_instances[w] |= (1u << w_new_instance);
					    reorder_.push_back(
						make_periodic_vertex(w, w_new_instance)
					    );
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	    std::swap(vertex_instances_, vertex_instances);
	    insert_vertices(nb_vertices_phase_I, reorder_.size());
	}
    }

    void PeriodicDelaunay3d::check_volume() {
	ConvexCell C;
	C.use_exact_predicates(convex_cell_exact_predicates_);	
	
	Logger::out("Periodic") << "Checking total volume..." << std::endl;
	double sumV = 0;
	IncidentTetrahedra W;
	
	FOR(v, nb_vertices_non_periodic_) {
	    copy_Laguerre_cell_from_Delaunay(v, C, W);
#ifdef GEO_DEBUG	    
	    for(
		VBW::ushort t = C.first_triangle();
		t!=VBW::END_OF_LIST; t=C.next_triangle(t)
	    ) {
		for(index_t lv=0; lv<3; ++lv) {
		    // Make sure there is no vertex at infinity
		    geo_debug_assert(C.triangle_v_local_index(t,lv) != 0);
		}
	    }
#endif	    
	    C.compute_geometry();
	    sumV += C.volume();
	}

	double expectedV = period_*period_*period_;
	
	Logger::out("Periodic") << "Sum volumes = " << sumV << std::endl;
	Logger::out("Periodic") << "  (expected " <<  expectedV << ")"
				<< std::endl;

	if(::fabs(sumV - expectedV) / expectedV >= 1.0 / 10000.0) {
	    Logger::err("Periodic") << "FATAL, volume error is too large"
				 << std::endl;
	    exit(-1);
	}
	
    }

    void PeriodicDelaunay3d::save_cells(
	const std::string& basename, bool clipped
    ) {
	static int index = 1;

	std::string index_string = String::to_string(index);
	while(index_string.length() < 3) {
	    index_string = "0" + index_string;
	}
	
	std::ofstream out((basename+index_string+".obj").c_str());
	index_t v_off = 1;
	
	ConvexCell C;
	C.use_exact_predicates(convex_cell_exact_predicates_);		
	IncidentTetrahedra W;
	for(index_t vv=0; vv<nb_vertices_non_periodic_; ++vv) {
	    copy_Laguerre_cell_from_Delaunay(vv, C, W);
	    if(clipped) {
		C.clip_by_plane(vec4( 1.0, 0.0, 0.0,  0.0));
		C.clip_by_plane(vec4(-1.0, 0.0, 0.0,  period_));
		C.clip_by_plane(vec4( 0.0, 1.0, 0.0,  0.0));
		C.clip_by_plane(vec4( 0.0,-1.0, 0.0,  period_));	
		C.clip_by_plane(vec4( 0.0, 0.0, 1.0,  0.0));
		C.clip_by_plane(vec4( 0.0, 0.0,-1.0,  period_));
	    }
	    v_off += C.save(out, v_off, 0.1);
	}
	++index;
    }
}


/******* extracted from ../basic/common.cpp *******/



#include <sstream>
#include <iomanip>

#ifdef GEO_OS_EMSCRIPTEN
#include <emscripten.h>
#endif

namespace GEO {

    void initialize(int flags) {
	static bool initialized = false;

	if(initialized) {
	    return;
	}
	
        // When locale is set to non-us countries,
        // this may cause some problems when reading
        // floating-point numbers (some locale expect
        // a decimal ',' instead of a '.').
        // This restores the default behavior for
        // reading floating-point numbers.
#ifdef GEO_OS_UNIX
        setenv("LC_NUMERIC","POSIX",1);
#endif

#ifndef GEOGRAM_PSM						
        Environment* env = Environment::instance();
        env->set_value("version", VORPALINE_VERSION);
        env->set_value("release_date", VORPALINE_BUILD_DATE);
        env->set_value("SVN revision", VORPALINE_SVN_REVISION);        
#endif
	
        Logger::initialize();
        Process::initialize(flags);
        Progress::initialize();
        CmdLine::initialize();
        PCK::initialize();
        Delaunay::initialize();

#ifndef GEOGRAM_PSM		
	Biblio::initialize();
#endif
        atexit(GEO::terminate);

#ifndef GEOGRAM_PSM	
        mesh_io_initialize();
#endif
	
        // Clear last system error
        errno = 0;

#ifndef GEOGRAM_PSM		
        // Register attribute types that can be saved into files.
        geo_register_attribute_type<Numeric::uint8>("bool");                
        geo_register_attribute_type<char>("char");        
        geo_register_attribute_type<int>("int");
        geo_register_attribute_type<unsigned int>("unsigned int");	
        geo_register_attribute_type<index_t>("index_t");
        geo_register_attribute_type<signed_index_t>("signed_index_t");	
        geo_register_attribute_type<float>("float");
        geo_register_attribute_type<double>("double");

        geo_register_attribute_type<vec2>("vec2");
        geo_register_attribute_type<vec3>("vec3");
#endif
	
#ifdef GEO_OS_EMSCRIPTEN
        
        // This mounts the local file system when an emscripten-compiled
        // program runs in node.js.
        // Current working directory is mounted in /working,
        // and root directory is mounted in /root
        
        EM_ASM(
            if(typeof module !== 'undefined' && this.module !== module) {
                FS.mkdir('/working');
                FS.mkdir('/root');            
                FS.mount(NODEFS, { root: '.' }, '/working');
                FS.mount(NODEFS, { root: '/' }, '/root');
            }
        );
#endif

#ifndef GEOGRAM_PSM
        ImageLibrary::initialize() ;

        geo_declare_image_serializer<ImageSerializerSTBReadWrite>("png");
        geo_declare_image_serializer<ImageSerializerSTBReadWrite>("jpg");
        geo_declare_image_serializer<ImageSerializerSTBReadWrite>("jpeg");
        geo_declare_image_serializer<ImageSerializerSTBReadWrite>("tga");
        geo_declare_image_serializer<ImageSerializerSTBReadWrite>("bmp");
	
        geo_declare_image_serializer<ImageSerializer_xpm>("xpm") ;
        geo_declare_image_serializer<ImageSerializer_pgm>("pgm") ;		
#endif
	
	initialized = true;
    }

    void terminate() {
        if(
            CmdLine::arg_is_declared("sys:stats") &&
            CmdLine::get_arg_bool("sys:stats") 
        ) {
            Logger::div("System Statistics");
            PCK::show_stats();
            Process::show_stats();
        }

        PCK::terminate();

#ifndef GEOGRAM_PSM
        ImageLibrary::terminate() ;
	Biblio::terminate();
#endif
	
        Progress::terminate();
        Process::terminate();
        CmdLine::terminate();
        Logger::terminate();
        Environment::terminate();
    }
}

