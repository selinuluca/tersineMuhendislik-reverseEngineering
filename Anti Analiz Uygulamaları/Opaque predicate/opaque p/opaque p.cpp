#include <stdio.h>
#include <stdlib.h>

int opaque_condition(int value) {
    // Değişmeyecek bir koşul ayarlayarak false bir belirsizlik oluşturuyoruz.
    return (value * value + 5) % 2 == 0;
}

int main() {
    int sensitive_data = 42;

    // Sabit bir koşulla false bir dallanma ekliyoruz, her zaman `true` olacak şekilde ayarlı.
    if (opaque_condition(15)) {
        sensitive_data += 10;
        printf("Sensitive data işlem görüyor: %d\n", sensitive_data);
    }
    else {
        // Bu blok asla çalıştırılmayacak, bizi yanıltmak için burada.
        sensitive_data -= 10;
        printf("Bu bloğa girilmiyor!\n");
    }

    // İkinci bir opaque predicate ile koşul ekliyoruz, yine her zaman aynı sonucu verecek.
    if ((sensitive_data * 2) % 2 == 0 && !opaque_condition(sensitive_data)) {
        sensitive_data ^= 0x55;
        printf("Sensitive data güncelleniyor: %d\n", sensitive_data);
    }
    else {
        // Bu blok da çalıştırılmayacak, bizi yanıltmak için burada. 
        printf("Sensitive data aynı kaldı: %d\n", sensitive_data);
    }

//böylece kodun her iki yöne gidebilirmiş gibi görünmesi sağlanırken, gerçekte sadece if blokları çalıştırılarak 
//her zaman aynı sonucu verecek fakat dallanma analizini zorlaştıracak şekilde tasarlanmış oldu.
//Koşulların sonucunda sensitive_data değerine yapılan XOR işlemi(sensitive_data ^= 0x55) kodun zararlı işleyişini ortaya çıkaran veya 
//kritik bir işlem gerçekleştiren adımı olarak düşünülebilir.Bu işlemle veri şifrelenebilir, gizli bir komut tetiklenebilir veya daha karmaşık bir zararlı davranış başlatılabilir.

    return 0;
}
