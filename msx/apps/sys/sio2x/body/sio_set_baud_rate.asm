; sio_set_baud:
; 	ld	a, l
; 	cp	h
; 	jr	nc, .skip1
; 	ld	l, h					; h is larger than l, so use that

; .skip1:
; 	ld	a, (RS_SIO_CLK)
; 	ld	d, a
; 	ld	ix, baud_mapping_table
; 	ld	bc, baud_clock_map_size

; .loop
; 	ld	a, (ix+baud_clock_map.sio_clk_code)

; 	cp	d					; is this entry for the sio/2 clock?
; 	jr	z, .found_entry

; .try_next
; 	add	ix, bc
; 	jr	.loop

; .found_entry:
; 	ld	a, (ix+baud_clock_map.fossil_max_code)
; 	cp	l					; is it <= to fossil_max_code
; 	jr	c, .try_next 				; no, try next record

; 	; found our entry

; 	ld	a, (ix+baud_clock_map.sio_clk_divider)
; 	ld	(RS_SIO_CLK_DIV), a
; 	ld	c, (ix+baud_clock_map.fossil_sel_code)	; get the baud rate selected

; 	ld	l, c					; return the actual selected baud rate
; 	ld	h, c
; 	RET
