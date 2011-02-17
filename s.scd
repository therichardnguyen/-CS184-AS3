(Include fish1 "poly.obj")

(G g1
	(I fisha1 fish1 (T -3 4))
	(I fisha2 fish1 (T 3 4))
	(I fisha3 fish1 (T 3 -4))
)

(Render g1)