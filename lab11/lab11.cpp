﻿#include <iostream>

int main() {

    setlocale(LC_ALL, "");  // налаштування локалі для коректного виведення
    int x = 0;       // Початкове значення x
    double y = 0.0;  // Для збереження результату обчислення

    const double const_1_5 = 1.5;
    const double const_31_6 = 31.6;
    const double const_45 = 45.0;
    const double const_400 = 400.0;

    __asm {
    find_x:
        // Обчислюємо x^2
        mov eax, x                           // Завантажуємо x в регістр eax
            imul eax, eax                        // eax = x^2

            // Конвертуємо результат x^2 в плаваюче число та зберігаємо в регістр
            cvtsi2sd xmm0, eax                   // Перетворюємо x^2 в double і зберігаємо в xmm0

            // Обчислюємо 1.5 * x^2
            movsd xmm1, qword ptr[const_1_5]     // Завантажуємо 1.5 в xmm1
            mulsd xmm0, xmm1                     // xmm0 = 1.5 * x^2

            // Обчислюємо 31.6 * x
            cvtsi2sd xmm1, x                     // Перетворюємо x в double і зберігаємо в xmm1
            movsd xmm2, qword ptr[const_31_6]    // Завантажуємо 31.6 в xmm2
            mulsd xmm1, xmm2                     // xmm1 = 31.6 * x

            // Додаємо до 1.5 * x^2 вираз 31.6 * x
            addsd xmm0, xmm1                     // xmm0 = 1.5 * x^2 + 31.6 * x

            // Додаємо 45
            movsd xmm1, qword ptr[const_45]      // Завантажуємо 45 в xmm1
            addsd xmm0, xmm1                     // xmm0 = 1.5 * x^2 + 31.6 * x + 45

            // Зберігаємо результат у змінній y
            movsd qword ptr[y], xmm0             // Зберігаємо результат у y

            // Перевіряємо, чи y перевищує 400
            movsd xmm0, qword ptr[y]             // Завантажуємо y в xmm0
            movsd xmm1, qword ptr[const_400]     // Завантажуємо 400 в xmm1
            ucomisd xmm0, xmm1                   // Порівнюємо y і 400

            // Якщо y <= 400, збільшуємо x і повторюємо
            jbe increment_x                      // Якщо y <= 400, переходимо до increment_x

            // Вихід з циклу, якщо y > 400
            jmp end_program                      // Вихід з програми

            increment_x :
        inc x                                // Збільшуємо x на 1
            jmp find_x                           // Повертаємося до циклу

            end_program :
    }

    // Виводимо результат
    std::cout << "Найменше значення X, при якому sqrt(y) > 20: " << x << std::endl;

    return 0;
}

/*
НЕ РОБИТЬ з Умовні переходи FPU
ПОМИЛКА 
ВИВОДИТЬ ВСЕ ВРЕМЯ 0

#include <iostream>

int main() {
    int x = 0;       // Початкове значення x
    double y = 0.0;  // Для збереження результату обчислення

    const double const_1_5 = 1.5;
    const double const_31_6 = 31.6;
    const double const_45 = 45.0;
    const double const_400 = 400.0;

    __asm {
    find_x:
        // Обчислюємо x^2
        fild dword ptr[x]                    // Завантажуємо x в FPU стік
        fmul st(0), st(0)                    // st(0) = x^2

        // Обчислюємо 1.5 * x^2
        fld qword ptr[const_1_5]             // Завантажуємо 1.5 в FPU
        fmul st(1), st(0)                    // st(1) = 1.5 * x^2 (результат у st(1))

        // Обчислюємо 31.6 * x
        fild dword ptr[x]                    // Завантажуємо x в FPU
        fld qword ptr[const_31_6]            // Завантажуємо 31.6 в FPU
        fmul                                // st(0) = 31.6 * x

        // Додаємо 1.5 * x^2 до 31.6 * x
        fadd st(1), st(0)                    // st(1) = 1.5 * x^2 + 31.6 * x
        fstp st(0)                           // Вивантажуємо st(0)

        // Додаємо 45
        fld qword ptr[const_45]              // Завантажуємо 45 в FPU
        fadd                                // st(0) = 1.5 * x^2 + 31.6 * x + 45

        // Зберігаємо результат у змінній y
        fstp qword ptr[y]                    // Вивантажуємо результат у y

        // Перевіряємо, чи y перевищує 400
        fld qword ptr[y]                     // Завантажуємо y у FPU
        fld qword ptr[const_400]             // Завантажуємо 400 в FPU
        fcomi st(0), st(1)                   // Порівнюємо y і 400
        fstp st(0)                           // Вивантажуємо результат порівняння

        // Якщо y <= 400, збільшуємо x і повторюємо
        jbe increment_x                      // Якщо y <= 400, переходимо до increment_x

        // Вихід з циклу, якщо y > 400
        jmp end_program                      // Вихід з програми

        increment_x :
        inc x                                // Збільшуємо x на 1
        jmp find_x                           // Повертаємося до циклу

        end_program :
    }

    // Виводимо результат
    std::cout << "The smallest integer x for which y exceeds 400 is: " << x << std::endl;

    return 0;
}


*/