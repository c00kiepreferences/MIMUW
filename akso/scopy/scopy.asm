global _start

section .data

SYS_OPEN equ 2
SYS_CLOSE equ 3
SYS_EXIT equ 60


buffer_size_in equ 32768                    ; Rozmiar bufora in.
buffer_size_out equ 65536                   ; Rozmiar bufora out - musi być 2 razy większy niż in.

buffer_in times buffer_size_in db 0         ; Alokowanie bufora in.
buffer_out times buffer_size_out db 0       ; Alokowanie bufora out.

file_in times 4 db 0                        ; Pointer na plik in.
file_out times 4 db 0                       ; Pointer na plik out.

; r8 - Licznik pętli przechodzącej przez bufor.
; r9 - Długość wypełnienia bufora out.
; r10 - Długość podciągu niezawierającego s.

section .text

return_1:
    mov     eax, SYS_EXIT
    xor     edi, edi
    inc     edi                             ; Kod wyjścia: 1.
    syscall

_start:

check_parameters:
    cmp     qword [rsp], 3                  ; Łącznie z nazwą programu, powinny być 3 argumenty.
    jne     return_1                        ; Jeśli ilość argumentów się nie zgadza, return 1.

open_in:
    mov     eax, SYS_OPEN
    xor     esi, esi                        ; Flagi : O_RDONLY.
    mov     rdi, [rsp + 16]                 ; W rdi wskaźnik na wskaźnik na nazwę pliku in.
    syscall

    test    eax, eax                        ; Sprawdza wynik funkcji SYS_OPEN.
    jl      return_1                        ; Jeżeli ujemny, kończy program kodem 1.
    mov     [file_in], eax                  ; Inaczej, przechwytuje pointer na plik in.


create_out:
    mov     eax, SYS_OPEN
    mov     esi, 193                        ; Flagi: O_CREAT | O_WRONLY | O_EXCL.
    mov     edx, 00644Q 			        ; Tryb: -rw-r--r--.
    mov     rdi, [rsp + 24]                 ; W rdi wskaźnik na wskaźnik na nazwę pliku out.
    syscall

    xor     esi, esi                        ; Esi przez moment pełni funkcję zmiennej kontrolnej.
    dec     esi                             ; Ustawia esi na -1.
    test    eax, eax                        ; Sprawdza wynik SYS_OPEN.
    jl      close_in                        ; Jeśli SYS_OPEN się nie powiodło, kończy program.
                                            ; (Przy zakończeniu ważna jest wartość esi).
    mov     [file_out], eax                 ; Inaczej, przechwytuje pointer na plik out.


read:
    xor     r9, r9                          ; Zeruje licznik wypełnienia bufora out.
    xor     eax, eax 				        ; SYS_READ zostanie wykonane.
    mov     edi, [file_in] 				    ; Plik do odczytu.
    mov     rsi, buffer_in                  ; Bufor do którego wczytujemy dane.
    mov     rdx, buffer_size_in             ; Oczekiwana ilość wczytanych bajtów.
    syscall

    test    eax, eax                        ; Sprawdza wynik funkcji SYS_READ.
    mov     esi, eax                        ; Zapisuje wypełnienie bufora do esi.
    jl      fail                            ; Jeżeli ujemny, zamyka pliki i kończy z kodem 1.
    jne     process                         ; Plik się nie skończył, kontynuuje.
                                            ; Jeżeli plik się skończył:
    test    r10, r10
    je      exit                            ; Nie ma podciągu bez s do dopisania - kończy program.
    mov     [buffer_out + r9], r10          ; Inaczej, dopisuje długość tego podciągu.
    add     r9, 2                           ; Zwiększa długość bufora out.
    xor     r10, r10                        ; Zeruje długość podciągu bez s.
                                            ; Teraz program jeszcze raz wykona read, jednak
                                            ; tym razem opuści pętlę i wykona throw_buffer.

process:
    xor     r8, r8                          ; Zeruje licznik przechodzenia przez bufor in.
process_loop:
    cmp     esi, r8d                        ; Sprawdza, czy jest na końcu bufora in.
    jne     if_statements                   ; Jeśli nie, pomija wypisywanie bufora.
                                            ; Jeśli tak:
    xor     eax, eax
    inc     eax                             ; SYS_WRITE zostanie wykonane.
    mov     edi, [file_out] 				; Plik, do którego wpisuje.
    mov     esi, buffer_out                 ; Bufor, z którego wpisuje.
    mov     edx, r9d 				        ; Ilość bajtów do wpisania.
    syscall
    cmp     eax, r9d                        ; Sprawdza, czy wypisano oczekiwaną ilość bajtów.
    jne     fail 			                ; Jeżeli nie, zamyka pliki i kończy z kodem 1.
    jmp     read 			                ; Wchodzi w nową iterację pętli read.
if_statements:
    cmp     byte [buffer_in + r8], 's'      ; Sprawdza, czy "s".
    je      buffer_add_substr               ; Jeśli tak, aktualizuje buffer_out.
    cmp     byte [buffer_in + r8], 'S'      ; Sprawdza, czy "S".
    je      buffer_add_substr               ; Jeśli tak, aktualizuje buffer_out.
    inc     r10                             ; Jeśli adne zwiększa długość podciągu bez s
    jmp     end_of_loop                     ; i wykonuje continue.
buffer_add_substr:
    test    r10, r10
    je      buffer_add_sS                   ; Dla pustego podciągu przeskakuje do dopisania s.
    mov     [buffer_out + r9], r10w         ; Dopisuje długość podciągu.
    add     r9, 2                           ; Zwiększa długość bufora out.
    xor     r10, r10                        ; Zeruje długość podciągu.
buffer_add_sS:
    mov     al, [buffer_in + r8]
    mov     [buffer_out + r9], al           ; Wrzuca do bufora napotkane 's' lub 'S'.
    inc     r9                              ; Zwiększa długość bufora out.
end_of_loop:
    inc     r8                              ; Zwiększa licznik pętli process.
    jmp     process_loop                    ; Wraca na początek pętli process.


exit:
    xor     esi, esi                        ; Używa esi jako zmiennej kontrolnej.
    jmp     close_out                       ; Przeskakuje wrzucenie do esi kodu błędu.
fail:
    xor     rsi, rsi
    dec     rsi                             ; Jeżeli program nie zadziałał, ustawia esi na -1.
close_out:
    mov     eax, SYS_CLOSE
    mov     edi, [file_out] 		        ; Zamknie teraz plik out.
    syscall
    add     esi, eax                        ; Dodaje do esi ew kod błędu.
close_in:
    mov     eax, SYS_CLOSE
    mov     edi, [file_in] 				    ; Zamknie teraz plik in.
    syscall
    add     esi, eax                        ; Dodaje do esi ew kod błędu.
check_err:
    test    esi, esi                        ; Sprawdza, czy w esi nie ma żadnego kodu błędu.
    jne     return_1                        ; Jeśli jest, kończy program z kodem 1.

return_0:
    mov     eax, SYS_EXIT
    xor     edi, edi 				        ; Inaczej, kończy program z kodem 0.
    syscall