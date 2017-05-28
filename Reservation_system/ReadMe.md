Zápočtový projekt
 
Projekt: Rezervační Systém
Předmět: c++
Autor: Jaroslav Vedral 

***Uživatelská dokumentace***
-----------------------------

***Obecné informace***

Program slouží ke správě akcí a rezervací. Umožňuje ukládání, načítání,
vytváření akcí a rezervací. Program je konzolovou aplikací se základní nápovědou.
Pro zobrazení nápovědy stačí napsat ?. Podrobněji se i nápovědě dozvíte v sekci Nápověda.
Nedostatky programu najdete v sekci Chyby a problémy aplikace.
Programátorskou dokumentaci ve formě html stránek najdete na ve složce Documentation/html/index.html.
Pokud byste dali přednost raději man stránkám, tak man stránky najdete ve složce Documentation/man.

***Funkčnost***
Program se ovládá 5 základními příkazy: action, create, save, load, exit. Jako oddělovač mezi parametry se používá mezera.
Nesmí tedy již být použita kdekoliv jinde. Začneme těmi jednoduššími.

Příkaz CREATE dále má argumenty: jméno akce, maximální velikost sedadel, cena akce pro pořadatele.
Příkaz SAVE má dále argument jméno akce.
Příkaz LOAD má dále argument jméno akce.
Příkaz EXIT nemá již žádné argumenty.

Nyní se podíváme na složitější příkaz, konkrétně ACTION.
Tento příkaz má dále dvě možnosti, kudy se dále může ubírat.
První možnost je BUILDING a druhá je RESERVATIONS. BUILDING znamená, že chceme provádět operaci s akcí.
RESERVATIONS zase značí, že chceme provádět operaci s rezervacemi.

Operace, které můžeme provádět s akcí jsou následující: INSERT, REMOVE, RESIZE, INFO.
INSERT slouží ke vkládání buď řádků, nebo sloupců, což zvolíme následujícím parametrem row, nebo column.
Následně za sebou máme čtyři parametry příkazu.
Konkrétně hodnoty: jméno akce, pozice, za kterou budeme vkládat, počet vkládaných sedadel, cena vkládaných sedadel.

Parametr REMOVE slouží odstranění buď řádků nebo sloupců, což opět zvolíme následujícím parametrem row, nebo column.
Následně za sebou máme tři parametry příkazu. Konkrétně hodnoty: jméno akce, pozice, kde budeme mazat, počet mazaných sedadel.

Parametr RESIZE slouží k upravení maximální velikosti sedadel na akci a ceny za akci.
Za RESIZE následují tři hodnotové parametry: jméno akce, nový maximální počet sedadel, nová cena za akci.

Parametr INFO slouží k výpisu celkového počtu volných sedadel, seznamu volných sedadel, celkového zisku a ceny za jednotlivá sedadla.

Parametr RESERVATIONS, následující za action, značí, že chceme provádět operaci s rezervacemi.
Za ním následuje jeden ze tří parametrů: ADD, ALTER, REMOVE a FIND.

Parametr ADD slouží k přidání rezervace do akce.
Za parametrem ADD následují další 4 hodnotové parametry:
jméno akce, sedadla ve tvaru: ([rada;sloupec;cena][rada;sloupec;cena]...), osoba, na kterou je rezervace dělaná ve tvaru:
(Jmeno;Prijmeni;email), heslo pro úpravu rezervací.

Parametr ALTER slouží k editaci rezervace u konkrétní akce.
Za parametrem ALTER následují stejné hodnotové parametry jako u ADD.
 
Parametr REMOVE slouží k odebrání rezervace u konkrétní akce.
Za parametrem remove následují další 3 hodnotové parametry:
jméno akce, osoba, na kterou je rezervace dělaná ve tvaru:
(Jmeno;Prijmeni;email), heslo pro úpravu rezervací.

Za parametrem FIND následují další dva hodnotové parametry:
jméno akce, osoba, na kterou je rezervace dělaná ve tvaru: (Jmeno;Prijmeni;email).
Více se dozvíte v samotném programu pomocí ?.

***Nápověda***
Nápověda funguje následujícím způsobem. Po mezeře může následovat buď parametr, nebo otazník.
Pokud napíšeme otazník, provede se nám výpis následujících možných parametrů i s jejich popisy.
Pokud jsme napsali všechny parametry, které nám určují operaci a zbývají nám již pouze hodnotové parametry (ty, co reprezentují nějakou hodnotu),
můžeme napsat otazníků více za sebou oddělených mezerami a vypíší se nám takový počet hodnotových parametrů, jaký je počet otazníků. 
 
***Soubory a složka***
Všechny potřebné soubory se nachází ve workingDIR.
Soubor config.map v sobě nese databázi nápovědy.
Postupně tam budou vznikat soubory, do kterých se budou ukládat informace o rezervacích a akcích.
Takové soubory začínají vždy názvem a následuje text _seats|_reservations.dmp.
Též zde bude soubor název.data, který slouží jako dočasné uložiště akcí.
Celý program má též něco jako serializaci, sloužící k serializování po 5 provedených úpravách, aby se neztráceli data.
Více o serializaci v sekci serializace.

***Serializace***
Pro serializaci využíváme ukládání do dočasného souboru.
Po zaznamenání dat, počtu aktuálně platných dat a unikátního řetězce, který označuje konec zápisu platných dat do tohoto souboru,
který nese jméno filename.data, kde filename je jméno akce, se provede následně i append do souborů filename_seats|_reservations.dmp.
Díky uložení počtu platných dat a samotných dat do dočasného souboru máme udělanou záložní kopii.
Při případném přerušení pouze obnovíme v souboru filename_seats|_reservations.dmp platné data.
Což buď tedy přepíšeme chybná data a provedeme append, nebo zahodíme data v dočasném souboru, pokud neobsahuje značku konce dat.

***Chyby a problémy aplikace***
Aplikace má pár much. Jedna z nich je ztrátová serializace.
Pokud dojde k výpadku dat při přepisu do souboru s koncovkou dmp, nastane při pokusu tuto akci načíst obnova dat, která načte platná data a následně jimi přepíše celý soubor.
Ačkoli je to prováděno jediným příkazem a v binární formě, může zde nastat při tomto přepisu výjimka.
Chování v tomto případě určuje systém. Může se tedy stát, že se zapíše pouze část dat, nebo dokonce nic.
Pokud bude porušena struktura souboru, vše se objeví při pokusu příkazy zrekonstruovat.
Druhá moucha spočívá tak trochu v logice. Konkrétně opět při loadingu.
Při loadingu se pro jednodušší průběh rovnou vytváří instance akce v paměti.
Můžeme tedy vynechat například příkaz pro vytvoření akce, protože ta se sama vytvoří. Což samozřejmě nedává příliš smysl.
Nicméně alespoň základní ošetření při čtení operací s akcí zde je.
Nelze tedy přidávat řádky ani sloupce, atd, pokud není akce vytvořená.
Rezervace nicméně přidávat lze (přidání rezervace by mělo skončit chybou, neboť neexistují řádky).
