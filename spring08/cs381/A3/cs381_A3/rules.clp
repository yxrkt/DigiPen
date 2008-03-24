	(deffacts initial-data 
	 (start)
	)

	(defrule wild-guess
	 ?f1 <- (start)
	 =>
	 (retract ?f1)
   (guess 1 2 3 4)
	)

	(defrule done
	 (bulls ?x) 
	 (seq-size ?x) 
	 => 
	 (printout t "DONE!!!" crlf) 
	)

	(defrule bulls
	 (bulls ?x) 
	 => 
	 (printout t "BULLS " ?x crlf) 
	)

	(defrule cows
	 (cows ?x) 
	 => 
	 (printout t "COWS " ?x crlf) 
	)

	(deffunction print_all_facts () (facts))
