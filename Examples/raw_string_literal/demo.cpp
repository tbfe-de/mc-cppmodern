#include <iostream>
int main() {
    std::cout << u8R"!end-of-html!(
    <!DOCTYPE html>
    <html lang="de">  
        <head>
            <meta charset="utf-8" />
            <title>TBFE Homepage</title>
        </head>
        <body style="text-align: center;" >
            – Technische Beratung für EDV – <br />
            © 2016: Dipl.-Ing. Martin Weitzel, Roßdorf
        </body>
    </html>
    )!end-of-html!";
}

