	(deftemplate answer
	 (slot index)
	 (slot color)
	)
	
	(deffacts initial-data
	 (answer (index 0) (color -1))
	 (answer (index 1) (color -1))
	 (answer (index 2) (color -1))
	 (answer (index 3) (color -1))
	 (pbulls 0)
	 (cur-index 0)
	)
	
	(defrule guess_the_thing
	 (answer (index 0) (color ?c0))
	 (answer (index 1) (color ?c1))
	 (answer (index 2) (color ?c2))
	 (answer (index 3) (color ?c3))
	 =>
	 (guess ?c0 ?c1 ?c2 ?c3)
	)
	
	(defrule bulls-increased
	 ?ci-fact <- (cur-index ?ci)
	 ?pb-fact <- (pbulls ?pb)
	 (bulls ?b)
	 (test(> ?b ?pb))
	 =>
	 (retract ?pb-fact)
	 (assert (pbulls ?b))
	 (retract ?ci-fact)
	 (assert (cur-index (+ ?ci 1)))
	)
	
	(defrule bulls-not-increased
	 (cur-index ?i)
	 ?color-fact <- (answer (index ?i) (color ?c))
	 (pbulls ?pb)
	 (bulls ?b)
	 (test(eq ?b ?pb))
	 =>
	 (retract ?color-fact)
	 (assert (answer (index ?i) (color (+ ?c 1))))
	)
	
	(defrule done
	 (answer (index 0) (color ?c0))
	 (answer (index 1) (color ?c1))
	 (answer (index 2) (color ?c2))
	 (answer (index 3) (color ?c3))
	 (bulls ?b)
	 (test(eq ?b 4))
	 =>
	 (printout t "all bulls found" crlf)
	 (printout t "colors: " ?c0 " " ?c1 " " ?c2 " " ?c3 crlf)
	)

	(deffunction print_all_facts () (facts))