	#&print_ch_name #8 ECALLi 0 WRWi &print_ch
	#&message print_str
	HALT

@print_str
	DUP RDB DUP JZi &done
	IECALLi &print_ch
	ADDi 1
	JMPi &print_str
@done RET


[data]
	@print_ch_name "print_ch"
	@print_ch [resw]
	@message "Hello, world!" 10 0
