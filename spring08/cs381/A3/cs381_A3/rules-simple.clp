	(deftemplate answer
	 (slot index)
	 (slot color)
	)
	
	(deffacts initial-data
	 (answer (index 0) (color 0))
	 (answer (index 1) (color 0))
	 (answer (index 2) (color 0))
	 (answer (index 3) (color 0))
	)
	
	(defrule guess
	 (answer (index 0) (color ?c0))
	 (answer (index 1) (color ?c1))
	 (answer (index 2) (color ?c2))
	 (answer (index 3) (color ?c3))
	 =>
	 (guess_simple ?c0 ?c1 ?c2 ?c3)
	)
	
	(defrule bull-not-at-x
	 ?color-fact <- (answer (index ?i) (color ?c))
	 (not (bull-at ?i))
	 =>
	 (retract ?color-fact)
	 (assert (answer (index ?i) (color (+ ?c 1))))
	)
	
	(defrule done
	 (answer (index 0) (color ?c0))
	 (answer (index 1) (color ?c1))
	 (answer (index 2) (color ?c2))
	 (answer (index 3) (color ?c3))
	 (and (bull-at 0) (bull-at 1) (bull-at 2) (bull-at 3))
	 =>
	 (printout t "all bulls found" crlf)
	 (printout t "colors: " ?c0 " " ?c1 " " ?c2 " " ?c3 crlf)
	)

	(deffunction print_all_facts () (facts))
