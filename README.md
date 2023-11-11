# emb_week10
week10 emb
세그먼트 up/down counter

GPIO_driver / seg_driver
•Homewor
1. counte는 segments에 숫자를 출력하며, 0000~9999 까지 표시한다.
2. 0000에서 count down시 9999로, 9999에서 count up시 0000으로 이어진다.
3. keyboard [u], [d], ,[p], [q]로 각각 count up, count down, count setting, program quit로 동작한다.
4. 두개의 button으로 각각 count up, count down으로 동작한다.
5. segments와 button을 각각 별도의 커널모듈로 제작하고, 하나의 응용프로그램으로 실행한다.
