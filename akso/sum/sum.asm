global sum

section .text

; Argumenty funkcji sum:
;   rdi - int64_t wskaznik na tablice
;   rsi - size_t rozmiar tablicy
sum:
    mov     r8, 1
    xor     r11, r11                        ; W r11 jest pozycja do ktorej jestesmy znormalizowani.
    
for_tab:
    cmp     r8d, esi                        ; W r8d mamy obecny indeks tablicy (i).
    je      end                             ; Konczymy program, gdy dotrzemy do konca tablicy.

count_position:
    mov     rax, r8                         ; rax = i
    mul     r8                              ; rax *= i
    shl     rax, 6                          ; rax *= 64
    xor     rdx, rdx                        ; Zerowanie rdx do mnozenia.
    div     rsi                             ; rax /= n
                                            ; Teraz w rax znajduje sie offset dodawanej liczby
                                            ; wzgledem poczatku tablicy.

split_number:
    mov     r10, [rdi + 8 * r8]             ; r9 = x[i]
    mov     r9, r10                         ; r10 = x[i]
    mov     qword [rdi + 8 * r8], 0         ; x[i] = 0
    mov     cl, al                          ; cl = offset
    and     cl, 63                          ; cl %= 64, w cl jest teraz offset wzgledem
                                            ; pojedynczej komorki y (lokalny).
    sal     r9, cl                          ; W r9 mniej znaczace bity x[i].
    neg     cl                              ; cl = -cl
    add     cl, 64                          ; W cl jest teraz 64 - offset lokalny.
    cmp     cl, 64
    jne     shift_r10                       ; Jezeli cl != 64, po prostu je shiftuje o cl.
    dec     cl                              ; W przeciwnym wypadku, shiftuje o 63 bajty.
shift_r10:    
    sar     r10, cl                         ; W r10 sa teraz bardziej znaczace bity.

start_normalize:
    shr     rax, 6                          ; W rax pozycja tablicy do ktorej dodamy prefiks (k).
    add     rax, 2                          ; Teraz w rax k + 2 pozycja.
for_normalizing:
    cmp     r11, r8                         ; Sprawdza, czy y jest w calosci znormalizowane
    jae     add                             ; Jezeli tak, wyskakuje by nie naruszyc czesci
                                            ; tablicy z x.
    cmp     r11, rax                        ; Sprawdza, czy y jest znormalizowane do k + 2 pozycji.
    jae     add                             ; Jesli tak, wyskakuje z petli.
    mov     rcx, [rdi + 8 * r11]            ; W rcx ostatnia znormalizowana pozycja.
    sar     rcx, 63                         ; Jesli rcx ujemne, zmienia sie w -1, jesli
                                            ; dodatnie - w 0.
    mov     [rdi + 8 * r11 + 8], rcx        ; Normalizuje y wpisujac do nowego rekordu 0 lub -1.
    inc     r11                             ; Podbija indeks do ktorego znormalizowane jest y.
    jmp     for_normalizing                 ; Wraca do petli.

add:
    mov     rcx, r10                        ; W rcx bardziej znaczace bity
    sar     rcx, 63                         ; Jesli rcx ujemne, zmienia sie w -1, jesli
                                            ; dodatnie - w 0.
    cmp     r11, rax                        ; Sprawdza, czy y jest znormalizowane do k + 2 pozycji.
    je      both_normalized                 ; Jesli tak, uruchamia wariant dodawania
                                            ; do obu znormalizowanych pozycji
one_normalized:                             ; W przeciwnym wypadku, wariant z tylko mlodsza pozycja.
    add     [rdi + 8 * rax - 16], r9        ; Dodaje mniej znaczaca czesc.
    adc     [rdi + 8 * rax - 8], r10        ; Dodaje bardziej znaczaca czesc.
    jmp     for_tab_end                     ; Pomija wariant obu znormalizowanych pozycji
both_normalized:
    add     [rdi + 8 * rax - 16], r9        ; Dodaje mniej znaczaca czesc.
    adc     [rdi + 8 * rax - 8], r10        ; Dodaje bardziej znaczaca czesc.
    adc     [rdi + 8 * rax], rcx            ; Dodaje ew. carry lub pozyczke.

for_tab_end:
    inc     r8d                             ; Zwieksza i.
    jmp     for_tab                         ; Wraca na poczatek petli.

end:
    ret
