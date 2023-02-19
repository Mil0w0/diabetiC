### Documentation Technique DiabetiC

## Contexte
I. Objectif	1

    Cette application a pour but d'installer un suivi personnel de la glycémie d'une personne diabétique. Elle est la version numérique d'un journal tenu à la main au quotidien par les personnes diabétiques.

II. Cible
 * les personnes diabétiques de tous les types (T1, T2, gestationel et autres)
 * les personnes s'occupant d'un individu diabétique (animal de compagnie, enfants)


III.  Délais
Decembre -> Février

## Points techniques	
I. Introduction 

    Toute notre progression a été enregistrée via git et partagée via github, notre repo :  
    https://github.com/Mil0w0/diabetiC.git

II. Fonctionnalités avancées

    a. Enregistrer sa glycémie
        i. Coté structure en C
    Lorsque l'utilisateur est connecté, il peut enregistrer sa glycémie. 
    Pour cela, on va lui demander, la valeur de sa glycémie et un commentaire à ajouter (optionnel).
    Le tout va être stocké dans une structure Entry.
    
    struct Entry {
    Entry *next;
    double value; //the glycemia
    char * comment; // an optional comment about the glycemia
    char * taken_at ; //date when the glycemia was taken (can't be null)
    int entries; //number of entries of the journal
    }

    Entry est un noeud d'une liste chainée comprenant chaque glycémie rentrée par l'utilisateur à une date taken_at.
    Le nombre entries est incrémenté à chaque nouvelle entrées (Entry).
    La date taken_at n'est pas renseigné et est NULL par défaut car elle sera set grâce au NOW() du SQL.

    Value est un double qui peut prendre plusieurs formes selon l'unité utilisé pour exprimer la glycémie (nmol/L, mg/dL, g/L). 
    
    On vérifie donc que : 
    * value > 0 (et si virgule il existe, la remplacer par un . <=pas encore développé)
    * entries > 0
    * taille de comment < 256 caractères.

        ii. Coté Base de données. 
    Une fois que la structure a été créé, on est donc sûr qu'il n'y pas d'erreur dans les données, on peut donc envoyer chaque valeur dans la BDD.

    Une table ENTRY avec en colonne les mêmes attributs (sauf next) que la struct Entry est créé et on INSERT les valeurs récupérer dans la struct.
    A noter que taken_at est bien rempli grâce à NOW();   

    b. Récupérer les données des glycémies venant de la BDD lors de la connexion de l'utilisateur (hors première connexion)

    Lorsque l'utilisateur ferme l'appli, ses données sont donc sauvegardées grâce à une base de donnée SQLITE. Lorsqu'il revient et se connecte, pour afficher ses dernières glycémies ou ajouter une nouvelle entry, on recréé la liste chainée en utilisant les fonctions addEntry() et createEntry(). Elles ont été créés de sorte qu'on puisse les utiliser pour créer des noeuds depuis des inputs utilisateur ou qu'on récupère des données de la bdd puis qu'on crée des noeuds avec ces données.

    c. Alerte si glycémie hors de la cible.
    Si la glycémie que l'utilisateur entre dans son journal est en dehors de la cible alors un message d'alerte le lui indique et le conseil sur la démarche à suivre.
    Il est possible de désactiver cette option dans les settings ou le config.txt.

    d. Unités de la glycémie.

    e. Affichage des glycémies
    Il a y plusieurs mode d'affichage des glycémies, pour la base il y a celle qui affiche toutes les glycémies de l'user mais il y d'autre possibilités:
	* Before, qui affiche toutes les glycémies avant une date donnée
	* After, qui affiche toutes les glycémies après une date donnée
	* Between, qui affiche toutes les glycémies entre deux dates données

    f. Enregister un user

        i. Côté structure en C
    Pour commencer deux choix se posent pour l'utilisateur de l'application, se connecter ou créent un nouvel utilisateur :

	- Connexion avec un utilisateur, le programme va demander dans un scanf de rentrer le nom d'utilisateur et le mot de passe allant avec le compte et l'enverra avec une requête SQL

	- Création du nouvel utilisateur, pareil que pour la connexion, le programme va demander à l'utilisateur d'entrer des valeurs (username, password et age) suite à cela le programme va envoyer plusieurs requêtes à la base de données
	  Premièrement nous allons vérifier que le nom d'utilisateur n'est pas déjà pris par quelqu'un, dans ce cas-là le programme demandera de changer la valeur d'entrée de username.
	  Ensuite une requête est envoyée pour pouvoir enregister l'utilisateur dans la base USERS (crée précédemment dans le programme et qui reste la même a chaque ouverture du programme grâce à un CREATE IF NOT EXIST)

    Ces deux choix mènent à la même sortie, le fait d'être connecté à l'application, cela permet de pouvoir afficher un différent menu et de se balader dans l'application.
    Le mot de passe est parsé pour le stocker de manière protéger dans la base de données

	ii. Côté Base de données
    Comme vu précédemment une table USERS est créé à la première instance de l'application, ce qui va permettre d'enregister des utilisateurs, qui sont eux-mêmes définis par :

	- Un ID, clé primaire de la table, étant donné que les noms d'utilisateurs sont uniques nous aurions pu aussi les mettre en clé primaire mais cela permet une meilleure visibilité dans la table

	- Un Username, qui permet d'identifier l'utilisateur par un nom, et de lui-même pouvoir se reconnecter au redémarrage de l'application

	- Un Password, permettant à l'utilisateur concerné d'être le seul à pouvoir se connecter à son compte

	- Un Age, pour l'instant facultatif mais pouvant permettre de proposer un language plus adapté à l'âge de l'utilisateur (enfant)

	- Une Date de Création, le moment exact de la création de l'utilisateur pouvant permettre l'analyse des glycémies avec le temps

    Les actions que l'on fait sur cette table sont assez basiques allants de l'INSERT INTO () VALUES () au SELECT WHERE assez basique mais nécessaire pour le fonctionnement du menu de base

     g. Admin et Debug
     Pour la gestion niveau admin, l'app crée au premier lancement de l'application un user dans la bdd avec des identifiants admin qui va permettre d'utiliser d'autres fonctionnalités
     Lors de la connexion avec l'admin un nouveau menu est proposé qui permet d'afficher le contenu des bdds


III. Utilisation de la base de données

    L'application utilise SQLite, une base de donnée, comme système de persistence des données.

    * Choix de la bdd 
    Nous avons décidé d'utiliser SQLite principalement car notre application n'a pas besoin de passer par un serveur. L'application fonctionne en local sur un appareil alors stocker les informations sur la machine du client est plus rapide bien que cela pose le problème de perte des données si le client perd l'accès à sa machine. De plus, nous n'avons pas besoin de stocker une large quantité de données ni d'envoyer plus d'une requête à la fois. Il n'est donc pas nécessaire d'utiliser une base de données comme MySql qui demanderait plus de ressources même si nous sommes plus familier avec.

    * utilisation d'une requête - interraction entre le C et la BDD.
    SQLite est développé en C, language également utilisé pour notre application, ce qui facilite son utilisation. La librairie sqlite utilisée n'est pas intégrée par défaut alors il faut l'intégrer à notre projet mais cela reste une manipulation peu complexe. c.f docs/how_to_use_sqlite3_C_interface.

    * Pour chaque requête SQL on peut passer par une méthode rapide avec le sqlexec() ou une méthode plus personnalisé avec un prepare. Cette deuxième méthode nous permet d'utiliser des paramètres, faisant le lien entre SQL et le code.


## Mode d’utilisation :
I. UTILISATEURS

    ● Création d'un compte
        Renseigner un login, age, mot de passe et une glycémie cible.
        
    ● Connexion à son compte

    ● Utiliser le journal des glycémies
        * Ajouter des glycémies
        * Lire ses dernières glycémies et les trier
        * Connaitre la moyenne de ses glycémies
        * Voir le récap des hypoglycémies et hyperglycémies
    
    ● Utilisation du config.txt
	* Possibilités d'entre ses identifiants pour une connexion automatique à l'appli
	* Modification de l'unité de la glycémie qui est g/L par défaut
    
    ● MOdifications des paramètres du compte
	* Modifier la glycémie cible
	* Supprimer son compte


II. EN MODE DEBUG

    Un utilisateur par défaut est créé au démarrage de l'appli. Il s'agit de l'administrateur.
    Login : admin
    Password: Respons11
    Avec cet utilisateur, il est possible d'afficher les données de toute la table USERS et GLYCEMIA. 
    On peut imaginer qu'il est possible de supprimer un autre utilisateur depuis ce menu également.

## Synthèse des membres de l’équipe && répartition des tâches
● Loriane : 

    * Travaux sur les listes chaînées avec les glycémies 
    * Envoi des données liées à la glycémie à la bdd avec des paramètres
    * Bug solver
    * Rédaction des documentations, git owner.

    La liberté du sujet nous a permis de réaliser une petite application fonctionnelle dans la limite de nos capacités. 
    Elle est d'utilité publique et bien qu'il existe déjà des applications mobiles pour ce sujet, je me dis que quelqu'un pourra bien l'utiliser.
    
    Globalement, le développement de l'application a bien avancé mais a été entamé trop tard pour certaines fonctionnalités ce qui est dommage.
    Cependant diabetiC respecte les 4 grands critères évalués :
        * système de persistence des données : SQLITE
        * fichier de config au démarrage de l'appli : config.txt
        * des listes chainées : struct Entry
        * une librairie externe : sqlite3.h

    Petite pensée à Harry, el gato, cette app est pour toi.

    
● Marjorie :

● Nino :

	* Travaux sur la gestion de user, de la création à la modification, l'envoie en bdd...
	* Création d'un fichier de config, et récupération des données dans l'app
	* Fix de bugs, et combats avec l'app pour faire marcher certaines fonctionnalités
	* Création des menus et du squelette de l'application

	Le sujet nous a permis de revoir pas mal de notion de C et d'en apprendre des nouvelles, la librairie sqlite3 étant assez simple d'utilisation
	nous avons pu faire la majorité des fonctionnalités voulues.

	Je dédicace mon travail à Pom-Pom, le lapin, qui a vécu de sales histoires.
