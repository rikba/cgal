// Copyright (c) 2006  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
// 
// Author(s)     : Ron Wein     <wein@post.tau.ac.il>
//                 Iddo Hanniel <iddoh@cs.technion.ac.il>

#ifndef CGAL_ARR_BEZIER_CURVE_TRAITS_2_H
#define CGAL_ARR_BEZIER_CURVE_TRAITS_2_H

/*! \file
 * Definition of the Arr_Bezier_curve_traits_2 class. 
 */

#include <CGAL/tags.h>
#include <CGAL/Arr_traits_2/Bezier_curve_2.h>
#include <CGAL/Arr_traits_2/Bezier_point_2.h>
#include <CGAL/Arr_traits_2/Bezier_x_monotone_2.h>
#include <CGAL/Arr_traits_2/Bezier_cache.h>
#include <CGAL/Arr_traits_2/Bezier_bounding_rational_traits.h>

CGAL_BEGIN_NAMESPACE

/*! \class
 * A traits class for maintaining an arrangement of Bezier curves with
 * rational control points.
 *
 * The class is templated with four parameters: 
 * Rat_kernel A kernel that defines the type of control points.
 * Alg_kernel A geometric kernel, where Alg_kernel::FT is the number type
 *            for the coordinates of arrangement vertices and is used to
 *            represent algebraic numbers.
 * Nt_traits A number-type traits class. This class defines the Rational
 *           number type (should be the same as Rat_kernel::FT) and the
 *           Algebraic number type (should be the same as Alg_kernel::FT)
 *           and supports various operations on them.
 * Bounding_traits A traits class for filtering the exact computations.
 *                 By default we use the rational bounding traits.
 */
template <class RatKernel_, class AlgKernel_, class NtTraits_,
          class BoundingTraits_ = Bezier_bounding_rational_traits<RatKernel_> >
class Arr_Bezier_curve_traits_2 
{
public:

  typedef RatKernel_                             Rat_kernel;
  typedef AlgKernel_                             Alg_kernel;
  typedef NtTraits_                              Nt_traits;
  typedef BoundingTraits_                        Bounding_traits;
  typedef Arr_Bezier_curve_traits_2<Rat_kernel,
                                    Alg_kernel,
                                    Nt_traits,
                                    Bounding_traits>   Self;
 
  typedef typename Nt_traits::Integer            Integer;
  typedef typename Rat_kernel::FT                Rational;
  typedef typename Alg_kernel::FT                Algebraic;

  typedef typename Rat_kernel::Point_2           Rat_point_2;
  typedef typename Alg_kernel::Point_2           Alg_point_2;
  
  // Category tags:
  typedef Tag_true                               Has_left_category;
  typedef Tag_true                               Has_merge_category;
  typedef Tag_false                              Has_boundary_category;

  // Traits-class types:
  typedef _Bezier_curve_2<Rat_kernel,
                          Alg_kernel,
                          Nt_traits,
                          Bounding_traits>             Curve_2;

  typedef _Bezier_x_monotone_2<Rat_kernel,
                               Alg_kernel,
                               Nt_traits,
                               Bounding_traits>        X_monotone_curve_2;

  typedef _Bezier_point_2<Rat_kernel,
                          Alg_kernel,
                          Nt_traits,
                          Bounding_traits>             Point_2;

  // Type definition for the vertical-tangnecy and intersection point cache.
  typedef _Bezier_cache<Nt_traits>                     Bezier_cache;

private:

  // Type definition for the bounded intersection points mapping.
  typedef typename X_monotone_curve_2::Intersection_map   Intersection_map;

  // Data members:
  Bezier_cache      *_cache;      // Caches vertical tangency points and
                                  // intersection points that have been
                                  // exactly computed.

  Intersection_map  *_inter_map;  // Mapping curve pairs to their intersection
                                  // points.

  bool               _owner;      // Does the traits class own its structures.

public:

  /*!
   * Default constructor.
   */
  Arr_Bezier_curve_traits_2 ()
  {
    _cache = new Bezier_cache;
    _inter_map = new Intersection_map;
    _owner = true;
  }
    
  /*!
   * Copy constructor.
   */
  Arr_Bezier_curve_traits_2 (const Self& tr) :
    _cache (tr._cache),
    _inter_map (tr._inter_map),
    _owner (false)
  {}

  /*!
   * Assignment operator.
   */
  Self& operator= (const Self& tr)
  {
    if (this == &tr)
      return (*this);

    _cache = tr._cache;
    _inter_map = tr._inter_map;
    _owner = false;
    return (*this);
  }

  /*!
   * Destructor.
   */
  ~Arr_Bezier_curve_traits_2 ()
  {
    if (_owner)
    {
      delete _cache;
      delete _inter_map;
    }
  }

  /// \name Functor definitions.
  //@{

  /*! \class Compare_x_2
   * The Compare_x_2 functor.
   */
  class Compare_x_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Compare_x_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Compare the x-coordinates of two points.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return LARGER if x(p1) > x(p2);
     *         SMALLER if x(p1) < x(p2);
     *         EQUAL if x(p1) = x(p2).
     */
    Comparison_result operator() (const Point_2& p1, const Point_2& p2) const
    {
      return (p1.compare_x (p2,
                            const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get a Compare_x_2 functor object. */
  Compare_x_2 compare_x_2_object () const
  {
    return (Compare_x_2 (_cache));
  }

  /*! \class Compare_xy_2
   * The Compare_xy_2 functor.
   */
  class Compare_xy_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Compare_xy_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Compares two points lexigoraphically: by x, then by y.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return LARGER if x(p1) > x(p2), or if x(p1) = x(p2) and y(p1) > y(p2);
     *         SMALLER if x(p1) < x(p2), or if x(p1) = x(p2) and y(p1) < y(p2);
     *         EQUAL if the two points are equal.
     */
    Comparison_result operator() (const Point_2& p1, const Point_2& p2) const
    {
      return (p1.compare_xy (p2,
                             const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get a Compare_xy_2 functor object. */
  Compare_xy_2 compare_xy_2_object () const
  {
    return (Compare_xy_2 (_cache));
  }

  /*! \class Construct_min_vertex_2
   * The Construct_min_vertex_2 functor.
   */
  class Construct_min_vertex_2
  {
  public:
    /*!
     * Get the left endpoint of the x-monotone curve (segment).
     * \param cv The curve.
     * \return The left endpoint.
     */
    const Point_2& operator() (const X_monotone_curve_2 & cv) const
    {
      return (cv.left());
    }
  };

  /*! Get a Construct_min_vertex_2 functor object. */
  Construct_min_vertex_2 construct_min_vertex_2_object () const
  {
    return Construct_min_vertex_2();
  }

  /*! \class Construct_max_vertex_2
   * The Construct_max_vertex_2 functor.
   */
  class Construct_max_vertex_2
  {
  public:
    /*!
     * Get the right endpoint of the x-monotone curve (segment).
     * \param cv The curve.
     * \return The right endpoint.
     */
    const Point_2& operator() (const X_monotone_curve_2 & cv) const
    {
      return (cv.right());
    }
  };

  /*! Get a Construct_max_vertex_2 functor object. */
  Construct_max_vertex_2 construct_max_vertex_2_object () const
  {
    return Construct_max_vertex_2();
  }

  /*! \class Is_vertical_2
   * The Is_vertical_2 functor.
   */
  class Is_vertical_2
  {
  public:
    /*!
     * Check whether the given x-monotone curve is a vertical segment.
     * \param cv The curve.
     * \return (true) if the curve is a vertical segment; (false) otherwise.
     */
    bool operator() (const X_monotone_curve_2& cv) const
    {
      return (cv.is_vertical());
    }
  };

  /*! Get an Is_vertical_2 functor object. */
  Is_vertical_2 is_vertical_2_object () const
  {
    return Is_vertical_2();
  }

  /*! \class Compare_y_at_x_2
   * The Compare_y_at_x_2 functor.
   */
  class Compare_y_at_x_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Compare_y_at_x_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Return the location of the given point with respect to the input curve.
     * \param cv The curve.
     * \param p The point.
     * \pre p is in the x-range of cv.
     * \return SMALLER if y(p) < cv(x(p)), i.e. the point is below the curve;
     *         LARGER if y(p) > cv(x(p)), i.e. the point is above the curve;
     *         EQUAL if p lies on the curve.
     */
    Comparison_result operator() (const Point_2& p,
                                  const X_monotone_curve_2& cv) const
    {
      // Iddo: Need to enhance point_position (maybe in the future we will
      //       store subdivided subcurves).
      return (cv.point_position (p, 
                                 const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get a Compare_y_at_x_2 functor object. */
  Compare_y_at_x_2 compare_y_at_x_2_object () const
  {
    return (Compare_y_at_x_2 (_cache));
  }

  /*! \class Compare_y_at_x_left_2
   * The Compare_y_at_x_left_2 functor.
   */
  class Compare_y_at_x_left_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Compare_y_at_x_left_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Compares the y value of two x-monotone curves immediately to the left
     * of their intersection point.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param p The intersection point.
     * \pre The point p lies on both curves, and both of them must be also be
     *      defined (lexicographically) to its left.
     * \return The relative position of cv1 with respect to cv2 immdiately to
     *         the left of p: SMALLER, LARGER or EQUAL.
     */
    Comparison_result operator() (const X_monotone_curve_2& cv1,
                                  const X_monotone_curve_2& cv2,
                                  const Point_2& p) const
    {
      return (cv1.compare_to_left (cv2, p,
                                   const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get a Compare_y_at_x_left_2 functor object. */
  Compare_y_at_x_left_2 compare_y_at_x_left_2_object () const
  {
    return (Compare_y_at_x_left_2 (_cache));
  }

  /*! \class Compare_y_at_x_right_2
   * The Compare_y_at_x_right_2 functor.
   */
  class Compare_y_at_x_right_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Compare_y_at_x_right_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Compares the y value of two x-monotone curves immediately to the right
     * of their intersection point.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param p The intersection point.
     * \pre The point p lies on both curves, and both of them must be also be
     *      defined (lexicographically) to its right.
     * \return The relative position of cv1 with respect to cv2 immdiately to
     *         the right of p: SMALLER, LARGER or EQUAL.
     */
    Comparison_result operator() (const X_monotone_curve_2& cv1,
                                  const X_monotone_curve_2& cv2,
                                  const Point_2& p) const
    {
      return (cv1.compare_to_right (cv2, p,
                                    const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get a Compare_y_at_x_right_2 functor object. */
  Compare_y_at_x_right_2 compare_y_at_x_right_2_object () const
  {
    return (Compare_y_at_x_right_2 (_cache));
  }

  /*! \class Equal_2
   * The Equal_2 functor.
   */
  class Equal_2
  {
  private:
    const Bezier_cache   *p_cache;

  public:

    /*! Constructor. */
    Equal_2 (const Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Check if the two x-monotone curves are the same (have the same graph).
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \return (true) if the two curves are the same; (false) otherwise.
     */
    bool operator() (const X_monotone_curve_2& cv1,
                     const X_monotone_curve_2& cv2) const
    {
      return (cv1.equals (cv2,
                          const_cast<Bezier_cache&> (*p_cache)));
    }

    /*!
     * Check if the two points are the same.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return (true) if the two point are the same; (false) otherwise.
     */
    bool operator() (const Point_2& p1, const Point_2& p2) const
    {
      return (p1.equals (p2,
                         const_cast<Bezier_cache&> (*p_cache)));
    }
  };

  /*! Get an Equal_2 functor object. */
  Equal_2 equal_2_object () const
  {
    return (Equal_2 (_cache));
  }

  /*! \class Make_x_monotone_2
   * The Make_x_monotone_2 functor.
   */
  class Make_x_monotone_2
  {
  private:
    Bezier_cache         *p_cache;

  public:

    /*! Constructor. */
    Make_x_monotone_2 (Bezier_cache *cache) :
      p_cache (cache)
    {}

    /*!
     * Cut the given Bezier curve into x-monotone subcurves and insert them
     * into the given output iterator.
     * \param cv The curve.
     * \param oi The output iterator, whose value-type is Object. The returned
     *           objects is a wrapper for an X_monotone_curve_2 object.
     * \return The past-the-end iterator.
     */
    template<class OutputIterator>
    OutputIterator operator() (const Curve_2& B, OutputIterator oi)
    {
      // Try to compute the bounds of the vertical tangency points.
      Bounding_traits                              bound_tr;
      typename Bounding_traits::Control_point_vec  cpts;

      std::copy (B.control_points_begin(), B.control_points_end(),
                 std::back_inserter(cpts));

      typedef std::pair<typename Bounding_traits::Bez_point_bound,
                        typename Bounding_traits::Bez_point_bbox>  Tang_pair;

      std::list<Tang_pair>                         tang_bounds;

      bound_tr.vertical_tangency_points (cpts, 0, 1, tang_bounds);

      // Go over the computed bounds approximated for the vertical tangency
      // points in increasing order of their t parameters, and construct
      // Point_2 objects from the bounded tangency points.
      std::list<Point_2>                            tang_points;
      bool                                          app_ok = true;

      while (! tang_bounds.empty())
      {
        // Locate the bound with minimal t-value.
        typename std::list<Tang_pair>::iterator   iter;
        typename std::list<Tang_pair>::iterator   it_min;

        it_min = iter = tang_bounds.begin();
        ++iter;
        while (iter != tang_bounds.end())
        {
          if (CGAL::compare (iter->first.t_min,
                             it_min->first.t_min) == SMALLER)
          {
            it_min = iter;
          }
          ++iter;
        }

        // Continue with the bound for the point with minimal t-value.
        const typename Bounding_traits::Bez_point_bound& bound = it_min->first;
        const typename Bounding_traits::Bez_point_bbox&  bbox = it_min->second;

        if (! bound.can_refine)
        {
          // If we cannot refine the vertical-tangency bound anymore, then
          // we failed to bound the vertical tangency point.
          // Iddo: In the future, we might want to use the info.
          app_ok = false;
          break;
        }

        // Construct an approximate vertical tangency point.
        Point_2   pt;

        if (bound.point_type == Bounding_traits::Bez_point_bound::RATIONAL_PT)
        {
          CGAL_assertion (CGAL::compare (bound.t_min, bound.t_max) == EQUAL); 
          Rational  t0 = bound.t_min;

          pt = Point_2 (B, t0);
        }
        else
        {
          pt.add_originator (typename Point_2::Originator(B, bound));
        }
        pt.set_bbox (bbox);

        tang_points.push_back(pt);

        // Remove the tangency point with minimal t-value from the list.
        tang_bounds.erase (it_min);
      }

      // If bounding the approximated vertical-tangency points went fine,
      // use these points as endpoint for the x-monotone subcurves.
      if (app_ok)
      {
        // Create the x-monotone subcurves with approximate endpoints.
        typename std::list<Point_2>::const_iterator pit;
        Point_2    p0(B, Rational(0)); // A rational start point.

        for (pit = tang_points.begin(); pit != tang_points.end(); ++pit)
        {
          *oi = CGAL::make_object (X_monotone_curve_2 (B, p0, *pit,
                                                       *p_cache));
          ++oi;

          p0 = *pit;
        }

        Point_2    p1(B, Rational(1)); // A rational end point.

        *oi = CGAL::make_object (X_monotone_curve_2 (B, p0, p1,
                                                     *p_cache));

        return (oi);
      }

      // If we reached here then we have to compute the vertical-tangency
      // points in an exact manner. We do this by considering all t-values
      // on B(t) = (X(t), Y(t)), such that X'(t) = 0.
      const typename Bezier_cache::Vertical_tangency_list&
        vt_list = p_cache->get_vertical_tangencies (B.id(),
                                                    B.x_polynomial(),
                                                    B.x_norm());

      // Create the x-monotone subcurves.
      Point_2                                        p0 (B, Rational(0));
      Point_2                                        p1;
      typename Bezier_cache::Vertical_tangency_iter  it;

      for (it = vt_list.begin(); it != vt_list.end(); ++it)
      {
        p1 = Point_2 (B, *it);
        *oi = CGAL::make_object (X_monotone_curve_2 (B, p0, p1,
                                                     *p_cache));
        ++oi;
        p0 = p1;
      }

      // Create the final subcurve.
      p1 = Point_2 (B, Rational(1));
      *oi = CGAL::make_object (X_monotone_curve_2 (B, p0, p1,
                                                   *p_cache));

      return (oi);
    }
  };

  /*! Get a Make_x_monotone_2 functor object. */
  Make_x_monotone_2 make_x_monotone_2_object ()
  {
    return (Make_x_monotone_2 (_cache));
  }

  /*! \class Split_2
   * The Split_2 functor.
   */
  class Split_2
  {
  public:
    /*!
     * Split a given x-monotone curve at a given point into two sub-curves.
     * \param cv The curve to split
     * \param p The split point.
     * \param c1 Output: The left resulting subcurve (p is its right endpoint).
     * \param c2 Output: The right resulting subcurve (p is its left endpoint).
     * \pre p lies on cv but is not one of its end-points.
     */
    void operator() (const X_monotone_curve_2& cv, const Point_2 & p,
                     X_monotone_curve_2& c1, X_monotone_curve_2& c2) const
    {
      cv.split (p, c1, c2);
      return;
    }
  };

  /*! Get a Split_2 functor object. */
  Split_2 split_2_object () const
  {
    return Split_2();
  }

  /*! \class Intersect_2
   * The Intersect_2 functor.
   */
  class Intersect_2
  {
  private:
    Bezier_cache         *p_cache;
    Intersection_map     *p_imap;

  public:

    /*! Constructor. */
    Intersect_2 (Bezier_cache *cache, Intersection_map *imap) :
      p_cache (cache),
      p_imap (imap)
    {}

    /*!
     * Find the intersections of the two given curves and insert them to the
     * given output iterator.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param oi The output iterator.
     * \return The past-the-end iterator.
     */
    template<class OutputIterator>
    OutputIterator operator() (const X_monotone_curve_2& cv1,
                               const X_monotone_curve_2& cv2,
                               OutputIterator oi)
    {
      return (cv1.intersect (cv2, *p_imap, *p_cache, oi));
    }
  };

  /*! Get an Intersect_2 functor object. */
  Intersect_2 intersect_2_object ()
  {
    return (Intersect_2 (_cache, _inter_map));
  }

  /*! \class Are_mergeable_2
   * The Are_mergeable_2 functor.
   */
  class Are_mergeable_2
  {
  public:
    /*!
     * Check whether it is possible to merge two given x-monotone curves.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \return (true) if the two curves are mergeable - if they are supported
     *         by the same line and share a common endpoint; (false) otherwise.
     */
    bool operator() (const X_monotone_curve_2& cv1,
                     const X_monotone_curve_2& cv2) const
    {
      return (cv1.can_merge_with (cv2));
    }
  };

  /*! Get an Are_mergeable_2 functor object. */
  Are_mergeable_2 are_mergeable_2_object () const
  {
    return Are_mergeable_2();
  }

  /*! \class Merge_2
   * The Merge_2 functor.
   */
  class Merge_2
  {
  public:
    /*!
     * Merge two given x-monotone curves into a single curve (segment).
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param c Output: The merged curve.
     * \pre The two curves are mergeable, that is they are supported by the
     *      same conic curve and share a common endpoint.
     */
    void operator() (const X_monotone_curve_2& cv1,
                     const X_monotone_curve_2& cv2,
                     X_monotone_curve_2& c) const
    {
      c = cv1.merge (cv2);
      return;
    }
  };

  /*! Get a Merge_2 functor object. */
  Merge_2 merge_2_object () const
  {
    return Merge_2();
  }

  //@}

  /// \name Functor definitions for the Boolean set-operation traits.
  //@{

  /*! \class Compare_endpoints_xy_2
   * The Compare_endpoints_xy_2 functor.
   */
  class Compare_endpoints_xy_2
  {
  public:

    /*!
     * Compare the endpoints of an $x$-monotone curve lexicographically.
     * (assuming the curve has a designated source and target points).
     * \param cv The curve.
     * \return SMALLER if the curve is directed right;
     *         LARGER if the curve is directed left.
     */
    Comparison_result operator() (const X_monotone_curve_2& cv)
    {
      if (cv.is_directed_right())
        return (SMALLER);
      else
	return (LARGER);
    }
  };

  /*! Get a Compare_endpoints_xy_2 functor object. */
  Compare_endpoints_xy_2 compare_endpoints_xy_2_object() const
  {
    return Compare_endpoints_xy_2();
  }

  /*! \class Construct_opposite_2
   * The Construct_opposite_2 functor.
   */
  class Construct_opposite_2
  {
  public:

    /*!
     * Construct an opposite x-monotone curve (with swapped source and target).
     * \param cv The curve.
     * \return The opposite curve.
     */
    X_monotone_curve_2 operator() (const X_monotone_curve_2& cv)
    {
      return (cv.flip());
    }
  };

  /*! Get a Construct_opposite_2 functor object. */
  Construct_opposite_2 construct_opposite_2_object() const
  {
    return Construct_opposite_2();
  }

  //@}
};

CGAL_END_NAMESPACE

#endif
