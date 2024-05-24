# Paralel-Siralama-Algoritmalari
Bu C kodu ile metin dosyasının satırlarını paralel sıralama teknikleri kullanılarak sıralayabilen bir uygulamadır. Uygulamanın Paralel MergeSort ve Paralel QuickSort sıralama algoritmalarını uygulayabilir. sıralama performansı çeşitli iş parçacığı sayılarıyla değerlendirilir. Sıralama sonuçları bir çıktı dosyasına yazdırılır.

programın terminal üzerinden çalıştırılma biçimi:

./mySort <inputfile> <outputfile> <# of threads> <algorithm>

Örnek olarak;

./mySort unsorted.txt output.txt 16 quick

Bu örnekte, unsorted.txt isimli dosya sıralanacak dosyayı, output.txt sıralı kelimelerin yazılacağı çıktı dosyasının adını ve 16 iş parçacığı sayısıdır. Quick ise kullanılacak olan sort algoritmasının adını göstermektedir.
