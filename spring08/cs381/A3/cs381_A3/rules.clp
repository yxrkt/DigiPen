	(deftemplate answer
	 (slot index)
	 (slot color)
	 (slot found)
	)
	
	(deffacts initial-data
	 (prev-bulls 0)
	 (waiting 0)
	 (answer (index 0) (color 0) (found 0))
	 (answer (index 1) (color -1)(found 0))
	 (answer (index 2) (color -1)(found 0))
	 (answer (index 3) (color -1)(found 0))
	)
	
	(defrule make_guess
	 (declare (salience 10))
	 (answer (index 0) (color ?c0))
	 (answer (index 1) (color ?c1))
	 (answer (index 2) (color ?c2))
	 (answer (index 3) (color ?c3))
	 =>
	 guess ?c0 ?c1 ?c2 ?c3
	)
	
	(defrule bulls-not-increased
	 ?color-fact <- (answer (index ?i)(color ?c)(found ?f))
	 (test(eq ?f 0))
	 (bulls ?b)
	 (prev-bulls ?pb)
	 (test(eq ?b ?pb ))
	 =>
	 (retract ?color-fact)
	 (assert (answer (index ?i) (color (+ ?c 1))(found ?f)))
	)
	
	(defrule waiting-for-next
	 ?ans-fact <- (answer(index ?i)(color ?c)(found ?f))
	 ?wait-fact <- (waiting ?w)
	 (test(eq ?w 1))
	 =>
	 (printout t "TEST" crlf)
	 (retract ?wait-fact)
	 (assert (waiting 0))
	 (retract ?ans-fact)
	 (assert(answer(index ?i)(color (+ ?c 1))))
	)
	
	(defrule bulls-increased
	 ?wait-fact <- (waiting ?w)
	 ?ans-fact <- (answer(index ?i)(color ?c)(found ?f))
	 (bulls ?b)
	 ?bulls-fact <- (prev-bulls ?pb)
	 (test(< ?pb ?b))
	 =>
	 (retract ?bulls-fact)
	 (assert (prev-bulls ?b))
	 (retract ?wait-fact)
	 (assert (waiting 1))
	 (retract ?ans-fact)
	 (assert (answer(index ?i)(color ?c)(found 1)))
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
	
	(defrule update
	 ?b-fact <-(bulls ?b)
	 ?pb-fact <- (prev-bulls ?pb)
	 =>
	 (printout t "bulls: " ?b " prev-bulls: " ?pb crlf)
	)

	(deffunction print_all_facts () (facts))
