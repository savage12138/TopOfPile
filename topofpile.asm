#     T o p  o f  P i l e
#
#
# This routine finds the color of the part on top of a pile.
# This assembly file can be simulate, test and debug it in MiSaSiM.
# The MiSaSiM simulator can be installed according to the 
# instructions at http://lindawills.ece.gatech.edu/misasim/.
# 
# Author: Yida Wang
# Last date modified: 10/6/2019

.data
Pile:  .alloc	1024
color: .word	0,0							# declare a color array with 8 bytes of 0
.text

TopOfPile:	addi	$1, $0, Pile			# point to array base
			swi	    545						# generate pile

          # declare variables
        	addi	$2, $1, 131 			# i = 131
          	addi	$5, $0, 6				# count = 6
			addi	$4, $0, -1				# R4 = -1
		  # increment i and check if current pixel is black
L2:		 	addi	$2, $2, 1				# i++
			lb		$3, 0($2)				# Pile[i]
			beq 	$3, $0, L2				# Pile[i] == 0, i++
		  # check if bottom is black	
			lb 		$7, 64($2)				# Pile[i + 64]
			beq 	$7, $0, L2 				# if Pile[i + 64] == 0, i++
		  # check if left is black
			lb 		$9, -1($2)				# Pile[i - 1]
			beq 	$9, $0, L2 				# if Pile[i - 1] == 0, i++
		  # check if right is black
			lb		$8, 1($2)				# Pile[i + 1]
			beq 	$8, $0, L2 				# if Pile[i + 1] == 0, i++
		  # determine the horizontal line is below the vertical line
			bne 	$7, $3, L3				# if Pile[i + 64] != Pile[i], check L3
 			beq 	$9, $3, L2				# if Pile[i - 1] == Pile[i], i++
 			beq 	$8, $9, SetFalse		# if Pile[i + 1] == Pile[i - 1], set color to false
 		  # determine the vertical line is below the horizontal line
L3:			lb		$8, -64($2)				# Pile[i - 64]
			beq 	$8, $0, L2 				# if Pile[i - 64] == 0, i++
			bne 	$7, $8, L2				# if Pile[i + 64] != Pile[i - 64], i++
 		  # blacklist the color that is under another color
SetFalse:   lb		$6, color($8)			# color[Pile[i + 1]] or color[Pile[i - 64]]
			beq 	$6, $4, L2 				# if color[Pile[i + 1]] or color[Pile[i - 64]] == -1, i++
			sb  	$4, color($8)			# if not, color[Pile[i + 1]] or color[Pile[i - 64]] = -1
		  # check if found 7 colors
L1:			addi    $5, $5, -1				# count--
			bne 	$5, $0, L2				# if count == 0	
			addi	$6, $0, 0				# j = 0
		  # extract the color that is not blacklisted
Loop:		addi	$6, $6, 1				# j++
			lb		$4, color($6)			# R4 = color[j]
			bne 	$4, $0, Loop			# if color[j] != 0, Loop
		  # store the TopColor in $2
Answer: 	add 	$2, $0, $6				# TopColor = j
			swi	546							# submit answer and check
		  
End:		jr	$31							# return to caller
