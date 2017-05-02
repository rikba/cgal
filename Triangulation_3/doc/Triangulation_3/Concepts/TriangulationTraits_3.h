
/*!
\ingroup PkgTriangulation3Concepts
\cgalConcept

The concept `TriangulationTraits_3` is the first template parameter of the class 
`Triangulation_3`. It defines the geometric objects (points, segments, 
triangles and tetrahedra) forming the triangulation together with a few 
geometric predicates and constructions on these objects : lexicographical 
comparison, orientation in case of coplanar points and orientation in space. 

\cgalHasModel All models of `Kernel`.

\sa `CGAL::Triangulation_3`
*/

class TriangulationTraits_3 {
public:

/// \name Types 
/// @{

/*!
The point type. It must be `DefaultConstructible`, 
`CopyConstructible` and `Assignable`. 
*/ 
typedef unspecified_type Point_3; 

/*!
The segment type. 
*/ 
typedef unspecified_type Segment_3; 

/*!
The tetrahedron type. 
*/ 
typedef unspecified_type Tetrahedron_3; 

/*!
The triangle type. 
*/ 
typedef unspecified_type Triangle_3; 

/*!
A constructor object that must provide the function operator

`Point_3 operator()(Point_3 p)`,

which simply returns p.

\note This constructor is necessary because `Regular_triangulation_3`
inherits `Triangulation_3` and will provide an overload of `Construct_point_3`
that strips the weight from a weighted point.
*/
typedef unspecified_type Construct_point_3;

/*!
A constructor object that must provide the function operator 

`Segment_3 operator()(Point_3 p, Point_3 q)`, 

which constructs a segment from two points. 
*/ 
typedef unspecified_type Construct_segment_3; 

/*!
A constructor object that must provide the function operator 

`Triangle_3 operator()(Point_3 p, Point_3 q, Point_3 r )`, 

which constructs a triangle from three points. 
*/ 
typedef unspecified_type Construct_triangle_3; 

/*!
A constructor object that must provide the function operator 

`Tetrahedron_3 operator()(Point_3 p, Point_3 q, Point_3 r, Point_3 s)`, 

which constructs a tetrahedron from four points. 
*/ 
typedef unspecified_type Construct_tetrahedron_3; 

/*!
A predicate object that must provide the function operator 

`Comparison_result operator()(Point_3 p, Point_3 q)`, 

which returns `EQUAL` if the two points are equal. Otherwise it must 
return a consistent order for any two points chosen in a same line. 
*/ 
typedef unspecified_type Compare_xyz_3; 

/*!
A predicate object that must provide the function operator 

`Orientation operator()(Point_3 p, Point_3 q, Point_3 r)`, 

which returns `COLLINEAR` if the points are collinear. Otherwise it must 
return a consistent orientation for any three points chosen in a same plane. 

*/ 
typedef unspecified_type Coplanar_orientation_3; 

/*!
A predicate object that must provide the function operator 

`Orientation operator()(Point_3 p, Point_3 q, Point_3 r, Point_3 s)`, 

which returns POSITIVE, if `s` lies on the positive side of the oriented 
plane `h` defined by `p`, `q`, and `r`, returns NEGATIVE if 
`s` lies on the negative side of `h`, and returns COPLANAR if `s` 
lies on `h`. 
*/ 
typedef unspecified_type Orientation_3; 

/// @} 

/// \name Creation 
/// @{

/*!
Default constructor. 
*/ 
Triangulation_traits_3(); 

/*!
Copy constructor. 
*/ 
Triangulation_traits_3(const Triangulation_traits_3 & tr); 

/// @} 

/// \name Operations 
/// The following functions give access to the predicate and construction objects:
/// @{

/*!

*/
Construct_point_3 construct_point_3_object();

/*!

*/
Construct_segment_3 construct_segment_3_object();

/*!

*/
Construct_triangle_3 construct_triangle_3_object();

/*!

*/ 
Construct_tetrahedron_3 construct_tetrahedron_3_object(); 

/*!

*/ 
Compare_xyz_3 compare_xyz_3_object(); 

/*!

*/ 
Coplanar_orientation_3 coplanar_orientation_3_object(); 

/*!

*/ 
Orientation_3 orientation_3_object(); 

/// @}

}; /* end TriangulationTraits_3 */

