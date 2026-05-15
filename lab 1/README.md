# Simple KPL Scanner

Day la mot scanner don gian viet bang C cho ngon ngu KPL. Scanner doc ma nguon KPL, tach chuoi ky tu thanh cac token, sau do in ra tung token kem vi tri dong-cot trong file nguon.

Thu muc `tests` chua cac file vi du `.kpl` va ket qua mong doi `.txt`.

## Cau truc du an

```text
.
|-- code/
|   |-- scanner.c      # May trang thai chinh de nhan dien token
|   |-- token.c/.h     # Dinh nghia TokenType, keyword va ham tao token
|   |-- reader.c/.h    # Doc tung ky tu tu file nguon, theo doi line/column
|   |-- charcode.c/.h  # Bang phan loai ky tu ASCII
|   `-- error.c/.h     # Ma loi va ham in thong bao loi
`-- tests/
    |-- example1.kpl
    |-- example2.kpl
    |-- example3.kpl
    |-- result1.txt
    |-- result2.txt
    `-- result3.txt
```

## Muc dich

Scanner la buoc dau cua mot compiler/interpreter. No khong phan tich ngu phap va cung khong chay chuong trinh KPL. Nhiem vu cua no chi la:

1. Doc file nguon theo tung ky tu.
2. Bo qua khoang trang va comment.
3. Nhan dien token nhu keyword, identifier, number, operator, dau cau.
4. Bao loi neu gap ky tu hoac hang ky tu khong hop le.
5. In danh sach token theo dang `dong-cot:TOKEN`.

Vi du voi file:

```kpl
Program Example1;
Begin
End.
```

Ket qua mong doi:

```text
1-1:KW_PROGRAM
1-9:TK_IDENT(Example1)
1-17:SB_SEMICOLON
2-1:KW_BEGIN
3-1:KW_END
3-4:SB_PERIOD
```

## Cac thanh phan chinh

### `reader.c`

Module nay quan ly input stream va vi tri doc hien tai.

- `openInputStream()` khoi tao luong doc tu `stdin`.
- `readChar()` doc ky tu tiep theo vao bien toan cuc `currentChar`.
- `lineNo` va `colNo` duoc cap nhat moi khi doc ky tu.
- `closeInputStream()` dong file sau khi scan xong.

Scanner phu thuoc vao ba bien toan cuc:

```c
extern int lineNo;
extern int colNo;
extern int currentChar;
```

### `charcode.c`

File nay tao bang `charCodes[256]` de phan loai tung ky tu ASCII thanh cac nhom:

- chu cai: `CHAR_LETTER`
- chu so: `CHAR_DIGIT`
- khoang trang: `CHAR_SPACE`
- toan tu: `+`, `-`, `*`, `/`, `<`, `>`, `=`, `!`
- dau cau: `,`, `.`, `;`, `:`, `'`, `(`, `)`
- ky tu khong ho tro: `CHAR_UNKNOWN`

Nho bang nay, `scanner.c` khong can so sanh truc tiep qua nhieu ky tu rieng le.

### `token.c` va `token.h`

`token.h` dinh nghia enum `TokenType`, gom:

- token co ban: `TK_IDENT`, `TK_NUMBER`, `TK_CHAR`, `TK_EOF`
- keyword: `KW_PROGRAM`, `KW_CONST`, `KW_VAR`, `KW_BEGIN`, `KW_END`, ...
- special symbol: `SB_PLUS`, `SB_MINUS`, `SB_ASSIGN`, `SB_LPAR`, `SB_RPAR`, ...

`token.c` co bang keyword va ham:

- `checkKeyword(char *string)`: kiem tra identifier co phai keyword khong.
- `makeToken(TokenType tokenType, int lineNo, int colNo)`: cap phat va tao token moi.

Keyword duoc so sanh khong phan biet hoa thuong, vi `keywordEq()` dung `toupper()`.

### `scanner.c`

Day la file trung tam cua scanner. Ham quan trong nhat la:

```c
Token* getToken(void)
```

![alt text](image.png)
- `state = 0`: trang thai bat dau, quyet dinh loai token dua tren `currentChar`.
- gap chu cai: chuyen sang trang thai doc identifier/keyword.
- gap chu so: chuyen sang trang thai doc number.
- gap dau toan tu/dau cau: chuyen sang trang thai tao symbol token.
- gap dau nhay don: doc hang ky tu dang `'x'`.
- gap `(`: co the la dau ngoac trai hoac bat dau comment `(* ... *)`.
- gap EOF: tra ve `TK_EOF`.

Ham `scan()` khoi tao input, goi `getToken()` lap lai cho den EOF, in token bang `printToken()`.

## Cac token duoc ho tro theo thiet ke

### Keyword

```text
PROGRAM CONST TYPE VAR INTEGER REAL CHAR ARRAY OF FUNCTION PROCEDURE
BEGIN END CALL IF THEN ELSE WHILE DO FOR TO
```

### Token co gia tri

```text
TK_IDENT   # ten bien, ten ham, ten chuong trinh
TK_NUMBER  # so nguyen
TK_REAL    # so thuc, vi du: 12.34
TK_CHAR    # hang ky tu mot ky tu, vi du: 'a'
TK_EOF     # ket thuc file
```

### Ky hieu dac biet

```text
;  :  .  ,  :=  =  !=  <  <=  >  >=
+  -  *  /  (  )  (.  .)
```

Trong enum, cac ky hieu nay co ten bat dau bang `SB_`, vi du `SB_SEMICOLON`, `SB_ASSIGN`, `SB_NEQ`, `SB_PLUS`.

## Dinh dang output

Moi token duoc in theo dang:

```text
line-column:TOKEN_NAME
```

Neu token co noi dung, noi dung nam trong ngoac:

```text
1-9:TK_IDENT(Example1)
7-12:TK_NUMBER(0)
27-22:TK_CHAR(' ')
```

## Cach build va chay

Neu da cai GCC/MinGW, co the bien dich tu thu muc goc:

```powershell
gcc code\scanner.c code\token.c code\reader.c code\charcode.c code\error.c -o scanner.exe
```

Sau do dua source code vao stdin:

```powershell
Get-Content -Raw tests\example1.kpl | .\scanner.exe
```

## Trang thai hien tai cua ma nguon

Phan con thieu trong `scanner.c` da duoc hoan thien theo automata trong `automata.png`:

- bo qua khoang trang
- nhan dien identifier va keyword
- nhan dien number, real number va kiem tra tran so nguyen
- nhan dien cac toan tu `+`, `-`, `*`, `/`, `=`, `!=`, `<`, `<=`, `>`, `>=`
- nhan dien dau `,`, `.`, `;`, `:`, `:=`, `(`, `)`
- nhan dien selector `(.` va `.)`
- bo qua comment `(* ... *)`
- nhan dien hang ky tu dang `'x'`

Mot so loi phu tro cung da duoc sua de code co the build/chay on dinh:

- dong nhat ten `CHAR_EXCLAMATION` trong `charcode.h`, `charcode.c` va `scanner.c`.
- `openInputStream()` trong `reader.c/.h` doc tu `stdin`.
- `Token` trong `token.h` co buffer `string` co kich thuoc ro rang.
- `main()` trong `scanner.c` nhan duong dan file nguon tu command line.

## Bo test

Thu muc `tests` gom:

- `example1.kpl`: chuong trinh rat ngan de kiem tra keyword, identifier, semicolon va period.
- `example2.kpl`: vi du factorial, co function, if/then/else, assignment, for loop va call.
- `example3.kpl`: vi du Tower of Hanoi, co procedure, nested call, `!=`, char constant va nhieu bieu thuc.

File `result1.txt`, `result2.txt`, `result3.txt` la output token mong doi tu scanner.

## Kiem tra ket qua

Sau khi build, co the chay scanner va so sanh output voi file ket qua mong doi:

```powershell
Get-Content -Raw tests\example1.kpl | .\scanner.exe > actual1.txt
fc actual1.txt tests\result1.txt
```

Lam tuong tu voi `example2.kpl` va `example3.kpl`.
