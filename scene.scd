(Include   fish1   "poly.obj")      ## this calls the polygon in that file "fish1"

(G school_A
    (I   fishA1   fish1   (S  0.5 0.5) (T  5  3.4) (color  1  0.0  0.0))     ## orange fish
    (I   fishA2   fish1   (S  0.7 0.4) (T  7.2  6))     						## uncolored fish
    (I   fishA3   fish1   (R  {t*2})   (T 8 9))     							## blank fish, rotating 2 degrees per frame around its center
 )

(G school_B
    (I   fishB1   fish1   (S  -0.5 0.5 )   (T 2 {0.4*sin(t*6*dgr)}))    ## mirrored fish, doing a complete vertical wiggle every 60 frames.
    (I   fishB2   fish1   (S {1 + 0.5*sin(t*3*dgr)}  0.6) (T 4 6) (color 0 1 0))    ## green fish, changing in length with a cycle of 120 frames.
 )

(G fishSchool
    (I   school_A1   school_A   (color 0 1 1))    				## cyan version of school_A
    (I   school_A2   school_A   (S -1 1) (color  1 0 1) (lod 0))    		## magenta, mirrored version of school_A
    (I   school_B1   school_B   (T {t*0.01} {t*0.01}) (lod  3))   ## school_B, slowly drifting diagonally off the view window, wire-frame only.
 )

(Render  fishSchool)


