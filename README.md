# Sosyal Ağ Analizi - Red-Black Tree Uygulaması


## 📌 Proje Açıklaması  
Red-Black Tree (Kırmızı-Siyah Ağaç) kullanarak geliştirdiğim bu proje, sosyal ağlardaki kullanıcı ilişkilerini analiz eder. Kullanıcıları verimli bir şekilde saklar ve aşağıdaki analizleri yapar.

## ✨ Temel Özellikler
✔️ **Kullanıcı Yönetimi**  
✔️ **Arkadaşlık İlişkileri** (Çift yönlü bağlantılar)  
✔️ **DFS/BFS Algoritmaları** ile ağ tarama  
✔️ **Ortak Arkadaş Bulma**  
✔️ **Etki Alanı Hesaplama** (BFS tabanlı)  
✔️ **Topluluk Tespiti** (Bağlı bileşenler)

---

## 🛠️ Kurulum

### Gereksinimler
- GCC Derleyici
- GNU Make

### Adımlar
```bash
# Repoyu klonla
git clone https://github.com/kullaniciadiniz/sosyal-ag-analizi.git

# Klasöre gir
cd sosyal-ag-analizi

# Programı derle
make

# Çalıştır
./sosyal_ag
```
## 📂 Veri Seti Formatı
data/ag_verisi.txt dosyasını aşağıdaki formatta hazırlayın:
```
USER 101
USER 102
USER 103
FRIEND 101 102
FRIEND 101 103
```
## 🧠 Algoritma Detayları
Red-Black Tree
```
void insert_user(User **root, User *node) {
  // Kullanıcıyı ağaca ekler ve dengeyi sağlar
}
```
DFS ile Arkadaş Bulma
```
void dfs(User *current, int depth, int target, List *result) {
  // Belirtilen derinlikteki kullanıcıları listeler
}
```
