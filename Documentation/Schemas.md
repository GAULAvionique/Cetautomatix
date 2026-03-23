# Cétautomatix

## 📡 Station au sol
- **Nom** : Station au sol (Ground Station)  
- **Description** : Interface opérateur pour contrôler et superviser la séquence de remplissage, purge et mise à feu.  
- **Fonctionnement** : Envoie les commandes vers le GSE via le module RF.  
- **Emplacement** : Située hors du banc d’essai, côté opérateur.  

---

## 🔗 Communications
- **Nom** : RFD900x
- **Description** : Modules radio de communication entre la station au sol et le GSE.  
- **Fonctionnement** : Transmettent les commandes (remplissage, purge, ignition) et les retours capteurs.  
- **Emplacement** :  
  - Un module au sol connecté à la station.  
  - Un module sur le GSE relié à l’électronique de commande.  

---

## 🔋 Batterie GSE
- **Nom** : Batterie (Alimentation station)  
- **Description** : Source d’énergie autonome pour le GSE.  
- **Fonctionnement** : Alimente le PCB GSE (STM32Fxx) et les valves associées.  
- **Emplacement** : Dans le rack GSE au sol.  

---

## 🧠 PCB GSE
- **Nom** : PCB GSE (STM32Fxx)  
- **Description** : Carte électronique de contrôle basée sur microcontrôleur STM32.  
- **Fonctionnement** :  
  - Reçoit les commandes du sol via RF.  
  - Contrôle l’ouverture/fermeture des valves.  
  - Lit les capteurs (pression, force).  
- **Emplacement** : Dans l’unité GSE.  

---

## 🟣 CO₂ Purge Tank
- **Nom** : Réservoir purge CO₂  
- **Description** : Réservoir contenant du CO₂ destiné à purger les conduites.  
- **Fonctionnement** :  
  - valve purge libère le CO₂ pour nettoyer les conduites du N₂O avant/pendant remplissage.  
- **Emplacement** : Sur le banc GSE, connecté en dérivation du circuit N₂O.  

---

## 🔵 N₂O Fill Tank
- **Nom** : Réservoir de remplissage N₂O  
- **Description** : Contient l’oxydant (protoxyde d’azote) utilisé pour remplir la fusée.  
- **Composants associés** :  
  - Capteur de pression (monitoring remplissage).  
  - valve Fill (permet le transfert vers le Flight Tank).  
- **Emplacement** : Sur le GSE, relié à la fusée par flexible.  

---

## ⚖️ Propellant Load Cell (Balance)
- **Nom** : Propellant Load Cell  
- **Description** : Capteur de force mesurant la masse de N₂O transférée dans la fusée.  
- **Fonctionnement** : La fusée repose sur cette cellule pendant le remplissage → permet de déduire la masse de propellant ajouté.  
- **Pertinence** : Utilisé aussi en vol indirectement, car permet de garantir la bonne quantité de N₂O avant le lancement.  
- **Emplacement** : Installée sur le rail de la fusée, sous la structure qui supporte la fusée.  

---

## 🔋 Batterie Moteur
- **Nom** : Batterie Moteur  
- **Description** : Source d’énergie embarquée pour la section propulsion de la fusée.  
- **Fonctionnement** : Alimente le PCB moteur et les valves du Flight Tank/IPA.  
- **Emplacement** : Intégrée dans la fusée, proche du moteur.  

---

## 🧠 PCB Moteur
- **Nom** : PCB Moteur (STM32Fxx)  
- **Description** : Carte de contrôle du moteur.  
- **Fonctionnement** :  
  - Reçoit commandes via UART depuis le GSE.  
  - Contrôle les valves (N₂O & IPA).  
  - Supervise l’igniter et le spark plug.  
- **Emplacement** : Dans le compartiment moteur de la fusée.  

---

## 🔵 N₂O Flight Tank
- **Nom** : Réservoir de vol N₂O  
- **Description** : Réservoir principal contenant le protoxyde d’azote pour la combustion.  
- **Composants associés** :  
  - Capteur de pression.  
  - valve Dump (vidange si nécessaire).  
  - valve Main (alimente la chambre de combustion).  
  - valve Igniter (alimente l’allumeur en N₂O).  
- **Emplacement** : Dans la fusée, relié au moteur.  

---

## 🔴 IPA Tank
- **Nom** : Réservoir IPA (Isopropanol)  
- **Description** : Contient le carburant (IPA).  
- **Composants associés** :  
  - valve Igniter (petit débit vers l’allumeur).  
  - valve Main (débit principal vers chambre de combustion).  
- **Emplacement** : Dans la fusée, à côté du réservoir N₂O.  

---

## 🔥 Igniter
- **Nom** : Allumeur  
- **Description** : Système déclencheur de l’allumage moteur.  
- **Composants associés** :  
  - Spark Plug (bougie d’allumage).  
  - Ignition Detector (capteur de continuité).  
- **Fonctionnement** : Injection IPA + N₂O en petite quantité → étincelle → amorce la combustion principale.  
- **Emplacement** : Juste en amont de la chambre de combustion.  

---

## 🚀 Thrust Chamber
- **Nom** : Chambre de combustion  
- **Description** : Partie où se mélangent et brûlent le N₂O et l’IPA.  
- **Composants associés** :  
  - Système de Tests (interface capteurs/monitoring).  
- **Emplacement** : Partie basse du moteur.  

---

## ⚖️ Thrust Load Cell (Tests)
- **Nom** : Thrust Load Cell  
- **Description** : Capteur de force type “pancake” utilisé pour mesurer la poussée lors des essais statiques.  
- **Fonctionnement** : Placé entre le moteur et la structure de test, mesure la force de poussée.  
- **Particularité** : Uniquement utilisée lors des tests au sol (pas en vol).  
- **Emplacement** : Banc d’essai moteur, sous la fusée.  

---

# 🔎 Remarques complémentaires
- **Sécurité** : L’ouverture complète des valves durant l’ignition est critique (annotation dans le schéma).  
- **Capteurs minimaux** :  
  - Pression sur réservoirs de N₂O (Fill et Flight).  
  - Balance propellant pour s’assurer de la bonne quantité de carburant avant le vol.  
  - Thrust load cell uniquement pour tests.  
- **Chaîne de contrôle** :  
  - Sol → RF → GSE STM32 → UART → PCB moteur → Action sur valves/igniter.  
- **Logique de séquence** :  
  1. Purge CO₂.  
  2. Remplissage N₂O.  
  3. Vérification masse/pression.  
  4. Ouverture valve main + ignition.  
  5. Combustion dans la chambre → poussée.  
