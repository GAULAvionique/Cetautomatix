# 🛤️ **Roadmap PCB Moteur Liquide & Station de Remplissage**

Plan du développement des deux PCB.

## 🎯 **Objectifs**
- [ ] Développer un PCB robuste pour le **moteur liquide**
- [ ] Créer un PCB dédié à la **station de remplissage automatisée**
- [ ] Intégrer la commande via **microcontrôleur STM32**
- [ ] Assurer la compatibilité avec capteurs et valves
- [ ] Assurer la communication entre le PCB Moteur Liquide et le PCB Station de Remplissage
- [ ] Assurer la communication entre le PCB Sation de Remplissage et la Station au Sol
- [ ] Intégration dans l'interface de la Station au Sol pour contrôle à distance
- [ ] Intégration des modules de sécurité avec arrêt d’urgence matériel / software  
- [ ] Réaliser un prototype fonctionnel et validé par tests

## 🛠️ **Conception du PCB**
- [ ] Définir les spécifications générales
  - [ ] Dimensions, puissance, contraintes thermiques, positionnement
- [ ] Sélectionner les composants
  - [ ] Microcontrôleur
  - [ ] Connecteurs
  - [ ] Protections électriques
- [ ] Créer les schémas électriques
  - [ ] Vérifier connexions et sécurité
- [ ] Concevoir le layout PCB
  - [ ] Placement et routage optimisé
- [ ] Commander et assembler un prototype
- [ ] Effectuer les premiers tests (moteur, valves, capteurs)

## 💻 **Programmation**
- [ ] Définir l’architecture logicielle de base
- [ ] Implémenter les fonctions de contrôle du moteur
- [ ] Gérer les entrées capteurs (niveau, pression)
- [ ] Implémenter le contrôle des valves
- [ ] Réaliser des tests unitaires et d’intégration

## 🌳 **Diagramme des Composantes**

- **Moteur Liquide**
  - [ ] Contrôle d'ouverture des valves
  - [ ] Lecture des capteurs de niveau et de pression
  - [ ] Vérification de continuité des igniters
  - [ ] Test de consommation et fiabilité
- **Station de Remplissage**
  - [ ] Gestion des valves
  - [ ] Lecture des capteurs de niveau et de pression
  - [ ] Test de consommation et fiabilité
- **Microcontrôleur (STM32)**
  - [ ] Initialisation des composants
  - [ ] Communication interne (UART, I2C, SPI)

## 📈 **Améliorations futures**
- Automatisation complète du cycle de remplissage  
- Intégration d’une interface web/app pour contrôle à distance  

---

🔙 Retour à la [page principale](../README.md)
