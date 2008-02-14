Alex Serio
Assignment 1
CS 381

The agent first looks for a node that is unexplored. If he finds one, he tries
to step there. Otherwise, he finds the node traversed the least. If there is a
tie, he will choose the node farthest from home. The distance from home for each
node is updated recursively when a new node is traversed. This lets the agent
return home fairly quick, especially if there are circuits in the path. The 
agent will return home after he reaches or surpasses a score about RETREAT_SCORE
(currently set to 70) or if he's performed RETREAT_TICKER actions in a row 
without cleaning (currently set to 150). These numbers were picked for best 
results for all tests (low retreat score to satisfy a room with only a few dirt
squares, large retreat tick count to allow him to reach dirt in room 6). Much
better results can be obtained in 6 by upping the retreat score a few hundred
points.

