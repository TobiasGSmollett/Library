main = print ()

-----Utils-----
eps :: (Floating a) => a
eps = 1e-8

add :: (Ord a, Floating a) => a -> a -> a
add a b
  | abs (a + b) < eps * (abs a + abs b ) = 0.0
  | otherwise = a + b

sgn :: (Ord a, Floating a) => a -> a -> Int
sgn a b
  | a < b - eps = -1
  | a > b + eps = 1
  | otherwise = 0

data Point a = Point { x :: a, y :: a } deriving (Eq)
instance (Show a) => Show (Point a) where
  show(Point x y) = show x ++ " " ++ show y

data Line a = Line {a :: Point a, b :: Point a}
instance (Show a) => Show (Line a) where
  show(Line a b) = show a ++ " " ++ show b

newtype Segment a = Segment { getSegment :: Line a}
data Circle a = Circle { center :: Point a, radius :: a }
newtype Polygon a = Polygon { getPolygon :: [Point a] }

-----Point-----
(^+) :: (Ord a, Floating a) => Point a -> Point a -> Point a
a ^+ b =  Point (add (x a) (x b)) (add (y a) (y b))

(^-) :: (Ord a, Floating a) => Point a -> Point a -> Point a
a ^- b = Point (add (x a) (-x b)) (add (y a) (-y b))

(^*) :: (Floating a) => Point a -> a -> Point a
a ^* d = Point (x a * d) (y a * d)

(^/) :: (Fractional a) => Point a -> a -> Point a
a ^/ d = Point (x a / d) (y a / d)

dot :: (Floating a) => Point a -> Point a -> a
a `dot` b = x a * x b + y a * y b

cross :: (Floating a) => Point a -> Point a -> a
a `cross` b = x a * y b - y a * x b

norm :: (Floating a) => Point a -> a
norm a = sqrt $ dot a a

dist :: (Ord a, Floating a) => Point a -> Point a -> a
dist a b = norm $ a ^- b

rotate :: (Floating a) => a -> Point a -> Point a -> Point a
rotate r a p = Point
  (cos (r * dx) - sin (r * dy) + x p)
  (sin (r * dx) + cos (r * dy) + y p)
  where dx = x a - x p
        dy = y a - y p

ccw :: (Ord a, Floating a) => Point a -> Point a -> Point a -> Int
ccw a b c
  | p0 `cross` p1 > 0 = 1
  | p0 `cross` p1 < 0 = -1
  | p0 `dot` p1 < 0 = 2
  | norm p0 < norm p1 = -2
  | otherwise = 0
  where p0 = b ^- a
        p1 = c ^- b

-----Line-----
on :: (Ord a, Floating a) => Point a -> Line a -> Bool
c `on` l = abs (ccw (a l) (b l) c) /= 1

project :: (Ord a, Floating a) => Line a -> Point a -> Point a
l `project` p = a l ^+ (base ^* t)
  where base = b l ^- a l
        t = ((p ^- a l) `dot` base) / (base `dot` base)

reflect :: (Ord a, Floating a) => Line a -> Point a -> Point a
l `reflect` p = p ^+ ((l `project` p) ^- p) ^* 2
-----Segment-----

-----Circle-----

-----Polygon-----
