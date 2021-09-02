;
;запуск системы при старте без дебагера
;
;
  .ref _c_int00

     .sect "codestart"
     LB _c_int00                        ;branch to start of code

	.end
