(Include   planet   "planet.obj") 
(Include   star  	"star.obj")  

(G starGroup
    (I   inst3   star   (R  {t*2})   (T 8 9)) 
    (I   inst1   star   (T  5  3.4))     ## orange fish
    (I   inst2   star   (T  7.2  6))     						## uncolored fish
 )


(G school_A
    (I   inst1   planet   (S  0.5 0.5) (T  5  3.4) (color  1  0.8  0.2))     ## orange fish
    (I   inst2   planet   (S  0.7 0.4) (T  7.2  6))     						## uncolored fish
    (I   inst3   star   (R  {t*2})   (T 8 9))     							## blank fish, rotating 2 degrees per frame around its center
 )

(G school_B
    (I   inst1   planet   (S  -0.5 0.5 )   (T 2 {0.4*sin(t*6*dgr)}))    ## mirrored fish, doing a complete vertical wiggle every 60 frames.
    (I   inst2   star   (S {1 + 0.5*sin(t*3*dgr)}  0.6) (T 4 6) (color 0 1 0))    ## green fish, changing in length with a cycle of 120 frames.
 )

(G spaceScene
    (I   i_1   starGroup   (color  1  0.8  0.2))    				## cyan version of school_A
    (I   i_2   school_A   (S -1 1) (color  1 0 1))    		## magenta, mirrored version of school_A
    (I   i_3   school_B   (T {t*0.01} {t*0.01}) (lod  2))   ## school_B, slowly drifting diagonally off the view window, wire-frame only.
 )

(Render  spaceScene)


