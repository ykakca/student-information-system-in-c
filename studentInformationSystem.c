#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

typedef struct DERS{
	int ders_id, kredi, kontenjan, ogrUye_id;
	char ders_isim[30];
}DERS;

typedef struct OGR_UYESI{
	int ogrUye_id;
	char ogrUye_ad[25], ogrUye_soyad[20], ogrUye_unvan[20];
}OGR_UYESI;

typedef struct OGRENCI{
	int ogr_id, ders_sayisi, toplam_kredi;
	char ogr_ad[20], ogr_soyad[20];
}OGRENCI;

typedef struct OGRDERSKAYIT{
	int ogr_id, ders_id, eslestirme_id;
	char tarih[20], durum[10];
}OGRDERSKAYIT;


OGRENCI* ogrenciTanimla(int id, int i, OGRENCI *ogrencilerDizisi);
DERS* dersTanimla(int id, int i, DERS *derslerDizisi);
OGR_UYESI* ogrUyeTanimla(int id, int i, OGR_UYESI *ogrUyeDizisi);
OGRENCI* ogrenciGuncelle(OGRENCI *ogrencilerDizisi, int index);
DERS* dersGuncelle(DERS *derslerDizisi, int index);
OGR_UYESI* ogrUyeGuncelle(OGR_UYESI *ogrUyeDizisi, int index);
OGRENCI* ogrenciSilme (int index, OGRENCI *ogrenciDizisi, OGRDERSKAYIT *kayitDizisi, int kayitIndex);
DERS* dersSilme (int index, DERS *dersDizisi, OGRDERSKAYIT *kayitDizisi, int kayitIndex);
OGR_UYESI* ogrUyeSilme (int index, OGR_UYESI *ogrUyeDizisi);
OGRDERSKAYIT* ogrDerseKaydet (OGRDERSKAYIT *kayitDizisi , int kayitIndex, int eslestirmeID, DERS *derslerDizisi, int dersIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex);
OGRDERSKAYIT* ogrDersiBirak(OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex, DERS *dersDizisi, int dersIndex);
FILE* ogrencileriDosyayaYaz(FILE *dosyaPtr, OGRENCI *ogrenciDizisi, int index);
FILE* dersleriDosyayaYaz(FILE *dosyaPtr, DERS *derslerDizisi, int index);
FILE* ogrUyeleriDosyayaYaz(FILE *dosyaPtr, OGR_UYESI *ogrUyeDizisi, int index);
FILE* kayitDosyasinaYaz(FILE *dosyaPtr, OGRDERSKAYIT *kayitDizisi, int index);
void ogrUyeDersListele (DERS* dersDizisi, int index, OGR_UYESI *ogrUyeDizi, int ogrUyeIndex);
void ogrAldigiDersleriListele (OGRDERSKAYIT *kayitDizisi, int kayitIndex, DERS *dersDizisi, int dersIndex);
void derseKayitliOgrListele (OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex);
void ogrUyesindenDersAlanlar (OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGR_UYESI *ogrUyeDizisi, int ogrUyeIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex);
void ogrDersKayitSilDersID (OGRDERSKAYIT *kayitDizisi, int kayitIndex, int id);
void ogrDersKayitSilOgrID(OGRDERSKAYIT *kayitDizisi, int kayitIndex, int id);


int main()
{
	printf("*********************YKUSIS Kayit Sistemi'ne Hosgeldiniz*********************\n");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int ogrID = 10000, ogrUyeID = 20000, dersID = 30000, eslestirmeID = 40000, ogrencilerIndex = 0, derslerIndex = 0, ogrUyeIndex = 0, kayitIndex = 0;
	FILE *ogrenciPtr = fopen("ogrenciler.txt","w+");
	FILE *dersPtr = fopen("dersler.txt","w+");
	FILE *ogrUyePtr = fopen("ogretimUyeleri.txt","w+");
	FILE *kayitPtr = fopen("OgrenciDersKayit.txt","w+");
	OGRENCI *ogrencilerDizisi = (OGRENCI*)malloc(sizeof(OGRENCI));
	DERS *derslerDizisi = (DERS*)malloc(100*sizeof(DERS));
	OGR_UYESI *ogrUyeDizisi = (OGR_UYESI*)malloc(sizeof(OGR_UYESI)*100);
	OGRDERSKAYIT *kayitDizisi = (OGRDERSKAYIT*)malloc(100*sizeof(OGRDERSKAYIT));
	
	if (ogrencilerDizisi == NULL || derslerDizisi == NULL || ogrUyeDizisi == NULL || kayitDizisi == NULL){
		printf("\nHata! Lutfen tekrar deneyiniz.");
		exit(0);
	}
	
	fprintf(ogrenciPtr, "*ID --- *Isim --- *Soyisim --- *Max. Ders Sayisi --- *Max. Kredi\n******************************************************************************************\n");
	fprintf(dersPtr, "*Kod --- *Ders Adi --- *Kontenjan --- *Kredi --- *Ogr. Uye ID'si\n******************************************************************************************\n");
	fprintf(ogrUyePtr, "*Isim --- *Soyisim --- *Unvan --- *ID\n******************************************************************************************\n");
	fprintf(kayitPtr, "*Ogrenci ID --- *Ders ID --- *Devam Durumu --- *Kayit Tarihi --- *Eslestirme ID'si\n******************************************************************************************\n");
	
	int anaOpr = 1, altOpr = 1;
	while (anaOpr >= 1 && anaOpr <= 4){
		printf("\nOgrenci/ders/egitmen tanimlamak icin 1'e, \nsilmek icin 2'ye,\nvar olan bir ogrenci/ders/egitmen uzerinde degisiklik yapmak icin 3'e,\nders kayit/goruntuleme islemleri icin 4'e, \ncikmak icin 0'a basiniz:");
		scanf("%d", &anaOpr);
		
		if (anaOpr == 1){
			altOpr = 1;
			while (altOpr >= 1 && altOpr <= 3){
				printf("\nOgrenci tanimlamak icin 1'e,\nders tanimlamak icin 2'ye,\nogretim uyesi tanimlamak icin 3'e,\nana menuye donmek icin 0'a basiniz: ");
				scanf("%d", &altOpr);		
				if (altOpr == 1){
					ogrencilerDizisi = ogrenciTanimla(ogrID, ogrencilerIndex, ogrencilerDizisi);
					ogrencilerIndex++;
					printf("Bilgilerini girdiginiz ogrencinin ID'si %d olarak belirlenmistir.\n",ogrID);
					ogrID++;
				}else if (altOpr == 2){
					derslerDizisi = dersTanimla(dersID, derslerIndex, derslerDizisi);
					derslerIndex++;
					printf("Bilgilerini girdiginiz dersin kodu %d olarak belirlenmistir.\n",dersID);
					dersID++;
				}else if (altOpr == 3){
					ogrUyeDizisi = ogrUyeTanimla(ogrUyeID, ogrUyeIndex, ogrUyeDizisi);
					ogrUyeIndex++;
					printf("Bilgilerini girdiginiz ogretim uyesinin ID'si %d olarak belirlenmistir.\n",ogrUyeID);
					ogrUyeID++;
				}else if (altOpr == 0){
					printf("\nAna ekrana donuluyor...\n");
				}else {
					printf("\nLutfen gecerli bir islem giriniz!\n");
					altOpr = 1;
				}
			}
		}
		else if (anaOpr == 2){
			altOpr = 1;
			while (altOpr >= 1 && altOpr <= 3){
				printf("\nOgrenci silmek icin 1'e,\nders silmek icin 2'ye,\nogretim uyesi silmek icin 3'e,\nana menuye donmek icin 0'a basiniz: ");
				scanf("%d", &altOpr);
				if (altOpr == 1){
					ogrencilerDizisi = ogrenciSilme(ogrencilerIndex, ogrencilerDizisi, kayitDizisi, kayitIndex);
					ogrencilerIndex--;
				}else if (altOpr == 2){
					derslerDizisi = dersSilme(derslerIndex, derslerDizisi, kayitDizisi, kayitIndex);
					derslerIndex--;
				}else if (altOpr == 3){
					ogrUyeDizisi = ogrUyeSilme(ogrUyeIndex, ogrUyeDizisi);
					ogrUyeIndex--;
				}else if (altOpr == 0){
					printf("\nAna ekrana donuluyor...\n");
				}else{
					printf("\nLutfen gecerli bir islem giriniz!\n");
					altOpr = 1;
				}
			}
		}
		else if (anaOpr == 3){
			altOpr = 1;
			while (altOpr >= 1 && altOpr <= 3){	
				printf("\nOgrenci kayitlarinda degisiklik yapmak icin 1'e,\nders kayitlarinda degisiklik yapmak icin 2'ye,\nogretim uyeleri kayitlarinda degisiklik yapmak icin 3'e,\nana menuye donmek icin 0'a basiniz: ");
				scanf("%d", &altOpr);		
				if (altOpr == 1){
					ogrencilerDizisi = ogrenciGuncelle(ogrencilerDizisi, ogrencilerIndex);					
				}else if (altOpr == 2){
					derslerDizisi = dersGuncelle(derslerDizisi, derslerIndex);					
				}else if (altOpr == 3){
					ogrUyeDizisi = ogrUyeGuncelle(ogrUyeDizisi, ogrUyeIndex);					
				}else if (altOpr == 0){
					printf("\nAna ekrana donuluyor...\n");
				}else{
					printf("\nLutfen gecerli bir islem giriniz!\n");
					altOpr = 1;
				}
			}
		}
		else if (anaOpr == 4){
			altOpr = 1;
			while (altOpr >= 1 && altOpr <= 6){
				printf("\nOgrenci ders kayit islemi icin 1'e,\ndersi birakmak icin 2'ye,\nbir ogr. uyesinin verdigi dersleri goruntulemek icin 3'e,\nbir ogrencinin aldigi dersleri goruntulemek icin 4'e,\nbir dersi alan ogrencileri goruntulemek icin 5'e,\nbir ogr. uyesinin sinifini goruntulemek icin 6'ya,\nana menuye donmek icin 0'a basiniz: ");
				scanf("%d", &altOpr);
				if (altOpr == 1){
					kayitDizisi = ogrDerseKaydet(kayitDizisi, kayitIndex, eslestirmeID, derslerDizisi, derslerIndex, ogrencilerDizisi, ogrencilerIndex);
					sprintf(kayitDizisi[kayitIndex].tarih, "%d-%02d-%02d/%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
					kayitIndex++;
					eslestirmeID++;
				}else if (altOpr == 2){
					kayitDizisi = ogrDersiBirak(kayitDizisi, kayitIndex, ogrencilerDizisi, ogrencilerIndex, derslerDizisi, derslerIndex);
					sprintf(kayitDizisi[kayitIndex].tarih, "%d-%02d-%02d/%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				}else if (altOpr == 3){
					ogrUyeDersListele (derslerDizisi, derslerIndex, ogrUyeDizisi, ogrUyeIndex);
				}else if (altOpr == 4){
					ogrAldigiDersleriListele(kayitDizisi, kayitIndex, derslerDizisi, derslerIndex);
				}else if (altOpr == 5){
					derseKayitliOgrListele(kayitDizisi, kayitIndex, ogrencilerDizisi, ogrencilerIndex);
				}else if (altOpr == 6){
					ogrUyesindenDersAlanlar(kayitDizisi, kayitIndex, ogrUyeDizisi, ogrUyeIndex, ogrencilerDizisi, ogrencilerIndex);
				}else if (altOpr == 0){
					printf("\nAna ekrana donuluyor...\n");
				}else{
					printf("\nLutfen gecerli bir islem giriniz!\n");
					altOpr = 1;
				}
			}
		}
		else if (anaOpr == 0){
			printf("\nProgram kapatiliyor...");
		}
		else{
			printf("\nLutfen gecerli bir islem giriniz!\n");
			anaOpr = 1;
		}
	}
	
	ogrenciPtr = ogrencileriDosyayaYaz(ogrenciPtr, ogrencilerDizisi, ogrencilerIndex);
	dersPtr = dersleriDosyayaYaz(dersPtr, derslerDizisi, derslerIndex);
	ogrUyePtr = ogrUyeleriDosyayaYaz(ogrUyePtr, ogrUyeDizisi, ogrUyeIndex);
	kayitPtr = kayitDosyasinaYaz(kayitPtr, kayitDizisi, kayitIndex);
	
	fclose(ogrenciPtr);
	fclose(dersPtr);
	fclose(ogrUyePtr);
	
	free(ogrencilerDizisi);
	free(derslerDizisi);
	free(ogrUyeDizisi);
	
	return 0;
}


OGRENCI* ogrenciTanimla(int id, int index, OGRENCI *ogrencilerDizisi){
	
	ogrencilerDizisi = (OGRENCI*)realloc(ogrencilerDizisi, ((index)+1)*sizeof(OGRENCI));
	printf("\nOgrencinin adini giriniz:");
	scanf("%s", ogrencilerDizisi[index].ogr_ad);
	printf("\nOgrencinin soyadini giriniz:");
	scanf("%s", ogrencilerDizisi[index].ogr_soyad);
	printf("\nOgrencinin aldigi ders sayisini giriniz:");
	scanf("%d", &ogrencilerDizisi[index].ders_sayisi);
	printf("\nOgrencinin alabilecegi toplam kredi sayisini giriniz: ");
	scanf("%d", &ogrencilerDizisi[index].toplam_kredi);

	ogrencilerDizisi[index].ogr_id = id;
	//*index++;
	//*id++;
	
	//fprintf(ogrenciPtr, "%s --- %s --- %d\n", yeniOgrenci->ogr_ad, yeniOgrenci->ogr_soyad, yeniOgrenci->ogr_id);
	printf("\nIsleminiz basariyla tamamlanmistir.\n");
	return ogrencilerDizisi;
}

DERS* dersTanimla(int id, int index, DERS *derslerDizisi){
	//DERS yeniDers;
	
	derslerDizisi = (DERS*)realloc(derslerDizisi, ((index)+1)*sizeof(DERS));	
	printf("\nDersin adini giriniz:");
	scanf("%s", derslerDizisi[index].ders_isim);
	printf("\nDersin kredisini giriniz:");
	scanf("%d", &derslerDizisi[index].kredi);
	printf("\nDersin kontenjanini giriniz:");
	scanf("%d", &derslerDizisi[index].kontenjan);
	printf("\nDersi veren ogretim uyesinin ID'sini giriniz: ");
	scanf("%d", &derslerDizisi[index].ogrUye_id);
	
	derslerDizisi[index].ders_id = id;


	//fprintf(dersPtr, "Dersin adi, IDsi, veren ogretim uyesinin IDsi, kredisi ve kontenjani bu sirayla girilmistir.");
	//fprintf(dersPtr, "%s --- %d --- %d --- %d --- %d\n", yeniDers->ders_isim, yeniDers->kontenjan, yeniDers->kredi, yeniDers->ogrUye_id);
	printf("\nIsleminiz basariyla tamamlanmistir.\n");
	return derslerDizisi;
}

OGR_UYESI* ogrUyeTanimla(int id, int index, OGR_UYESI *ogrUyeDizisi){
	//OGR_UYESI yeniOgrUye;
	
	ogrUyeDizisi = realloc(ogrUyeDizisi, ((index)+1)*sizeof(OGR_UYESI));
	printf("\nOgretim uyesinin adini giriniz:");
	scanf("%s", ogrUyeDizisi[index].ogrUye_ad);
	printf("\nOgretim uyesinin soyadini giriniz:");
	scanf("%s", ogrUyeDizisi[index].ogrUye_soyad);
	printf("\nOgretim uyesinin unvanini giriniz:");
	scanf("%s", ogrUyeDizisi[index].ogrUye_unvan);

	ogrUyeDizisi[index].ogrUye_id = id;


	//fprintf(ogrUyePtr, "%s --- %s --- %s --- %d\n", yeniOgrUye->ogrUye_ad, yeniOgrUye->ogrUye_soyad, yeniOgrUye->ogrUye_unvan, yeniOgrUye->ogrUye_id);
	printf("\nIsleminiz basariyla tamamlanmistir.\n");
	return ogrUyeDizisi;
}

OGRENCI* ogrenciGuncelle(OGRENCI *ogrencilerDizisi, int index){
	if (index <= 0){
		printf("\nIslem basarisiz! Hicbir ogrenci tanimlanmamis.\n");
	}else{
		int i = 0, ID;
		printf("\nBilgilerini degistirmek istediginiz ogrencinin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != ogrencilerDizisi[i].ogr_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return ogrencilerDizisi;
		}
		printf("\nOgrencinin adini giriniz: ");
		scanf("%s", &ogrencilerDizisi[i].ogr_ad);
		printf("\nOgrencinin soyadini giriniz: ");
		scanf("%s", &ogrencilerDizisi[i].ogr_soyad);
		printf("\nOgrencinin aldigi ders sayisini giriniz: ");
		scanf("%d", &ogrencilerDizisi[i].ders_sayisi);
		printf("\nOgrencinin alabilecegi toplam kredi sayisini giriniz: ");
		scanf("%d", &ogrencilerDizisi[i].toplam_kredi);
		printf("\nIsleminiz basariyla tamamlanmistir.\n");
	}
	return ogrencilerDizisi;
}

DERS* dersGuncelle(DERS *derslerDizisi, int index){
	if (index <= 0){
		printf("\nIslem basarisiz! Hicbir ders tanimlanmamis.\n");
	}else{
		int i = 0, ID;
		printf("\nBilgilerini degistirmek istediginiz dersin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != derslerDizisi[i].ders_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return derslerDizisi;
		}
		printf("\nDersin adini giriniz: ");
		scanf("%s", &derslerDizisi[i].ders_isim);
		printf("\nDersin kredisini giriniz: ");
		scanf("%d", &derslerDizisi[i].kredi);
		printf("\nDersin kontenjan sayisini giriniz: ");
		scanf("%d", &derslerDizisi[i].kontenjan);
		printf("\nDersi veren ogretim uyesinin ID'sini giriniz: ");
		scanf("%d", &derslerDizisi[i].ogrUye_id);
		printf("\nIsleminiz basariyla tamamlanmistir.\n");
	}
	return derslerDizisi;
}

OGR_UYESI* ogrUyeGuncelle(OGR_UYESI *ogrUyeDizisi, int index){
	if(index <= 0){
		printf("\nIslem basarisiz! Hicbir ogretim uyesi tanimlanmamis.\n");
	}else{
		int i = 0, ID;
		printf("\nBilgilerini degistirmek istediginiz ogretim uyesinin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != ogrUyeDizisi[i].ogrUye_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return ogrUyeDizisi;
		}
		printf("\nOgretim uyesinin adini giriniz: ");
		scanf("%s", &ogrUyeDizisi[i].ogrUye_ad);
		printf("\nOgretim uyesinin soyadini giriniz: ");
		scanf("%s", &ogrUyeDizisi[i].ogrUye_soyad);
		printf("\nOgretim uyesinin unvanini giriniz: ");
		scanf("%d", &ogrUyeDizisi[i].ogrUye_unvan);
		printf("\nIsleminiz basariyla tamamlanmistir.\n");
	}
	return ogrUyeDizisi;
}

OGRENCI* ogrenciSilme (int index, OGRENCI *ogrenciDizisi, OGRDERSKAYIT *kayitDizisi, int kayitIndex){
	if (index <= 0){
		printf("\nIslem basarisiz! Zaten hicbir ogrenci tanimlanmamis.\n");
	}else{
		int i, j, ID;
		printf("\nKaydini silmek istediginiz ogrencinin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != ogrenciDizisi[i].ogr_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return ogrenciDizisi;
		}
		for (j=i; j<index; j++){
			ogrenciDizisi[j] = ogrenciDizisi[j+1];
		}
		ogrenciDizisi = realloc(ogrenciDizisi, (((index)+1)*sizeof(OGRENCI)));
		ogrDersKayitSilOgrID(kayitDizisi, kayitIndex, ID);
		printf("\nIsleminiz basariyla tamamlanmistir.\n");	
	}
	return ogrenciDizisi;
}

DERS* dersSilme (int index, DERS *dersDizisi, OGRDERSKAYIT *kayitDizisi, int kayitIndex){
	if (index <= 0){
		printf("\nIslem basarisiz! Zaten hicbir ders tanimlanmamis.\n");
	}else{
		int i, j, ID;
		printf("\nKaydini silmek istediginiz dersin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != dersDizisi[i].ders_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return dersDizisi;
		}
		for (j=i; i<index; i++){
			dersDizisi[j] = dersDizisi[j+1];
		}
		dersDizisi = realloc(dersDizisi, (((index)+1)*sizeof(DERS)));
		ogrDersKayitSilDersID(kayitDizisi, kayitIndex, ID);
		printf("\nIsleminiz basariyla tamamlanmistir.\n");
	}
	return dersDizisi;
}

OGR_UYESI* ogrUyeSilme (int index, OGR_UYESI *ogrUyeDizisi){
	if (index <= 0){
		printf("\nIslem basarisiz! Zaten hicbir ogretim uyesi tanimlanmamis.\n");
	}else{
		int i, j, ID;
		printf("\nKaydini silmek istediginiz ogretim uyesinin ID'sini girin: ");
		scanf("%d", &ID);
		while (i <= index && ID != ogrUyeDizisi[i].ogrUye_id){
			i++;
		}
		if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return ogrUyeDizisi;
		}
		for (j=i; i<index; i++){
			ogrUyeDizisi[j] = ogrUyeDizisi[j+1];
		}
		ogrUyeDizisi = realloc(ogrUyeDizisi, (((index)+1)*sizeof(OGR_UYESI)));	
		printf("\nIsleminiz basariyla tamamlanmistir.\n");
	}
	return ogrUyeDizisi;
}

void ogrUyeDersListele (DERS* dersDizisi, int index, OGR_UYESI *ogrUyeDizi, int ogrUyeIndex){
	if (index <= 0){
		printf("\nIslem basarisiz! Hicbir ogretim uyesi tanimlanmamis.\n");
	}else{
		int i = 0, hocaID;
		printf("\nDersleri listelenecek ogretim uyesinin ID'sini giriniz: ");
		scanf("%d", &hocaID);
		while (i <= ogrUyeIndex && hocaID != ogrUyeDizi[i].ogrUye_id){
			i++;
		}if (i > index){
			printf("\nLutfen gecerli bir ID giriniz!\n");
		
		}else{
		
			printf("Verilen dersler:\n");
			for (i=0; i<=index; i++){
				if (hocaID == dersDizisi[i].ogrUye_id){
					printf("%s\n", dersDizisi[i].ders_isim);
				}
			}
		}
	}
}

OGRDERSKAYIT* ogrDerseKaydet (OGRDERSKAYIT *kayitDizisi , int kayitIndex, int eslestirmeID, DERS *derslerDizisi, int dersIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex){	
	int i = 0, j = 0, ogrenciID, dersID, tempKredi = 0, tempDersSayi = 1;;
	printf("\nKaydi yapilacak ogrencinin ID'sini girin: ");
	scanf("%d", &ogrenciID);
	printf("\nOgrencinin kaydedilecegi dersin ID'sini girin: ");
	scanf("%d", &dersID);
	
	while(i <= ogrenciIndex && ogrenciID != ogrenciDizisi[i].ogr_id){i++;}
	if (i > ogrenciIndex){
		printf("\nLutfen gecerli bir ID giriniz!\n");
		return kayitDizisi;
	}i = 0;
	while(i <= dersIndex && dersID != derslerDizisi[i].ders_id){i++;}
		if (i > dersIndex){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return kayitDizisi;
	}
	
	OGRENCI kayitOgrenci = ogrenciDizisi[i];
	while (dersID != derslerDizisi[j].ders_id && j <= dersIndex){
		j++;
	}if (j <= dersIndex) {tempKredi += derslerDizisi[j].kredi;}
			
	for (i=0; i <= kayitIndex; i++){
		if (kayitDizisi[i].ogr_id == ogrenciID){
			j = 0;
			while (kayitDizisi[i].ders_id != derslerDizisi[j].ders_id && j <= dersIndex){
				j++;
			}if (j <= dersIndex){tempKredi += derslerDizisi[j].kredi;}
		}
	}
	
	if (tempKredi > kayitOgrenci.toplam_kredi || tempDersSayi > kayitOgrenci.ders_sayisi){
		printf("\nIslem basarisiz! Ogrencinin alabilecegi toplam krediyi/toplam ders sayisini astiniz."); 
	}else{
		kayitDizisi = (struct OGRDERSKAYIT*)realloc(kayitDizisi, ((kayitIndex)+1)*sizeof(struct OGRDERSKAYIT));	
		kayitDizisi[kayitIndex].ders_id = dersID;
		kayitDizisi[kayitIndex].ogr_id = ogrenciID;
		kayitDizisi[kayitIndex].eslestirme_id = eslestirmeID;
		strncpy(kayitDizisi[kayitIndex].durum, "KAYITLI", 8);
		printf("\nIslem basariyla gerceklestirildi.\n");
	}
	return kayitDizisi;
}

OGRDERSKAYIT* ogrDersiBirak(OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex, DERS *dersDizisi, int dersIndex){
	if (kayitIndex <= 0){
		printf("\nIslem basarisiz! Hicbir ders kaydi tanimlanmamis.\n");
	}else{
		int i = 0, ogrID, dersID;
		printf("\nOgrencinin ID'sini giriniz: ");
		scanf("%d", &ogrID);
		
		while(i <= ogrenciIndex && ogrID != ogrenciDizisi[i].ogr_id){
			i++;
		}
		if (i > ogrenciIndex){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return kayitDizisi;
		}i = 0;
		
		printf("\nDersin ID'sini giriniz: ");
		scanf("%d",&dersID);
		while (i <= dersIndex && dersID != dersDizisi[i].ders_id){
			i++;
		}
		if (i > dersIndex){
			printf("\nLutfen gecerli bir ID giriniz!\n");
			return kayitDizisi;
		}
		
		
		while (i <= kayitIndex && dersID != kayitDizisi[i].ders_id){
			i++;
		}
		strncpy(kayitDizisi[i].durum, "BIRAKTI", 8);
	}
	return kayitDizisi;
}

void ogrDersKayitSilDersID(OGRDERSKAYIT *kayitDizisi, int kayitIndex, int id){
	int i;
	for (i=0; i<=kayitIndex; i++){
		if (id == kayitDizisi[i].ders_id){
			kayitDizisi[i].ders_id = 0;
			kayitDizisi[i].ogr_id = 0;
			kayitDizisi[i].eslestirme_id = 0;
			strncpy(kayitDizisi[i].durum,"-------",8);
			strncpy(kayitDizisi[i].tarih,"-------",8);
		}
	}
}

void ogrDersKayitSilOgrID(OGRDERSKAYIT *kayitDizisi, int kayitIndex, int id){
	int i;
	for (i=0; i<=kayitIndex; i++){
		if (id == kayitDizisi[i].ogr_id){
			kayitDizisi[i].ders_id = 0;
			kayitDizisi[i].ogr_id = 0;
			kayitDizisi[i].eslestirme_id = 0;
			strncpy(kayitDizisi[i].durum,"-------",8);
			strncpy(kayitDizisi[i].tarih,"-------",8);
		}
	}
}

void ogrAldigiDersleriListele (OGRDERSKAYIT *kayitDizisi, int kayitIndex, DERS *dersDizisi, int dersIndex){
	if (dersIndex <= 0 || kayitIndex <= 0){
		printf("\nIslem basarisiz! Hicbir ders/kayit tanimlanmamis.\n");
	}else{
		int i, j = 0, flag = 1, ogrenciID;
		printf("Aldigi dersler goruntulenecek ogrencinin ID'sini giriniz: ");
		scanf("%d", &ogrenciID);
		printf("Ogrencinin aldigi dersler:\n");
		for (i=0; i<=kayitIndex; i++){
			if (ogrenciID == kayitDizisi[i].ogr_id){
				while (j <= dersIndex && dersDizisi[j].ders_id != kayitDizisi[i].ders_id){
					j++;
				}
				if (j <= dersIndex){
					printf("%s\n", dersDizisi[j].ders_isim);
				}
				j = 0; flag = 1;
			}
		}
	}
}

void derseKayitliOgrListele (OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex){
	if (ogrenciIndex <= 0 || kayitIndex <= 0){
		printf("\nIslem basarisiz! Hicbir ogrenci/kayit tanimlanmamis.\n");
	}else{
		int i, j, dersID;
		printf("\nOgrencileri goruntulenecek dersin ID'sini giriniz: ");
		scanf("%d", &dersID);
		printf("\nDersi alan ogrenciler: ");
		for (i=0; i<=kayitIndex; i++){
			if (kayitDizisi[i].ders_id == dersID){
				j = 0;
				while (j <= ogrenciIndex && kayitDizisi[i].ogr_id != ogrenciDizisi[j].ogr_id){
					j++;
				}
				printf("\n%d,  %s %s", ogrenciDizisi[j].ogr_id, ogrenciDizisi[j].ogr_ad, ogrenciDizisi[j].ogr_soyad);
			}
		}
	}
}

void ogrUyesindenDersAlanlar (OGRDERSKAYIT *kayitDizisi, int kayitIndex, OGR_UYESI *ogrUyeDizisi, int ogrUyeIndex, OGRENCI *ogrenciDizisi, int ogrenciIndex){
	if (ogrUyeIndex <= 0 || ogrenciIndex <= 0 || kayitIndex <= 0){
		printf("\nIslem basarisiz! Hcibir ogretim uyesi/ogrenci/kayit tanimlanmamis.\n");
	}else{
		int i = 0, j = 0, k = 0, ogrID, dersID;
		char strDersID[30];
		printf("Ogretim uyesinin ID'sini giriniz: ");
		scanf("%d", &ogrID);
		while (i <= ogrUyeIndex && ogrID != ogrUyeDizisi[i].ogrUye_id){
			i++;
		}
		if (i > ogrUyeIndex){ 
			printf("\nLutfen gecerli bir ID giriniz!\n");
		}else{
			printf("Dersin kodunu/ID'sini giriniz:");
			scanf("%s", strDersID);
			dersID = atoi(strDersID);
			strncat(strDersID, "_DERSLISTESI.txt",17);
			FILE *sinifPtr = fopen(strDersID, "w+");
			for (j=0; j<=kayitIndex; j++){
				if (dersID == kayitDizisi[j].ders_id){
					k = 0;
					while (k <= ogrenciIndex && kayitDizisi[j].ogr_id != ogrenciDizisi[k].ogr_id){
						k++;
					}
					fprintf(sinifPtr, "%d, %s %s\n", ogrenciDizisi[k].ogr_id, ogrenciDizisi[k].ogr_ad, ogrenciDizisi[k].ogr_soyad);
				}
			}
		}
	}
}

FILE* ogrencileriDosyayaYaz(FILE *dosyaPtr,OGRENCI *ogrenciDizisi, int index){
	int i;
	for (i=0; i<index; i++){
		fprintf(dosyaPtr, "%d --- %s --- %s --- %d --- %d\n", ogrenciDizisi[i].ogr_id , ogrenciDizisi[i].ogr_ad, ogrenciDizisi[i].ogr_soyad, ogrenciDizisi[i].ders_sayisi, ogrenciDizisi[i].toplam_kredi);
	}
    return dosyaPtr;
	
}

FILE* dersleriDosyayaYaz(FILE *dosyaPtr,DERS *derslerDizisi, int index){
	int i;
	for (i=0; i<index; i++){
		fprintf(dosyaPtr, "%d --- %s --- %d --- %d --- %d\n", derslerDizisi[i].ders_id, derslerDizisi[i].ders_isim, derslerDizisi[i].kontenjan, derslerDizisi[i].kredi, derslerDizisi[i].ogrUye_id);
	}
	return dosyaPtr;
}

FILE* ogrUyeleriDosyayaYaz(FILE *dosyaPtr, OGR_UYESI *ogrUyeDizisi, int index){
	int i;
	for (i=0; i<index; i++){
		fprintf(dosyaPtr, "%s --- %s --- %s --- %d\n", ogrUyeDizisi[i].ogrUye_ad, ogrUyeDizisi[i].ogrUye_soyad, ogrUyeDizisi[i].ogrUye_unvan, ogrUyeDizisi[i].ogrUye_id);
	}
	return dosyaPtr;
}

FILE* kayitDosyasinaYaz(FILE *dosyaPtr, OGRDERSKAYIT *kayitDizisi, int index){
	int i;
	for (i=0; i<index; i++){
		fprintf(dosyaPtr, "%d --- %d --- %s --- %s --- %d\n", kayitDizisi[i].ogr_id, kayitDizisi[i].ders_id, kayitDizisi[i].durum,  kayitDizisi[i].tarih, kayitDizisi[i].eslestirme_id);
	}
}

