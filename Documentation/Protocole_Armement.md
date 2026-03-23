# 🔒 **Protocole d’Armement**

Ce document décrit la séquence d’armement et les états associés des différents dispositifs lors des phases critiques du remplissage et de l’allumage.

---

## 🗂️ **Légende des States**

- **CLOSED** → Valve fermée, aucun flux.  
- **OPENS** → Valve s’ouvre, flux autorisé.  
- **CLOSES** → Valve se ferme, flux interrompu.
- **UNPOWERED** → Dispositif non alimenté.  
- **POWERS (Burst)** → Alimentation activée brièvement.
- **UNPOWERS** → Alimentation coupée.
- **MONITORING** → Capteur actif et en surveillance continue.  
- **DETECTS FULL TANK** → Capteur détecte un réservoir plein.  
- **DETECTS IGNITION** → Capteur détecte l’allumage réussi.  

---

## ✅ **Séquences d'armement**

### #1 – Startup  
Mettre tous les systèmes dans leur état initial sécurisé.  

**GSE (Ground Support Equipment)**
| Device                          | State       |
| :---                            | :---        |
| Purge Valve                     | CLOSED      |
| N2O Fill Valve                  | CLOSED      |
| N2O Dump Valve                  | CLOSED      |
| Thrust Load Cell                | MONITORING  |
| Propellant Load Cell            | MONITORING  |
| N2O Fill Pressure Sensor        | MONITORING  |

**Moteur (Rocket)**
| Device                          | State       |
| :---                            | :---        |
| Spark Plug                      | UNPOWERED   |
| N2O Flight Pressure Sensor      | MONITORING  |
| N2O Igniter Valve               | CLOSED      |
| IPA Igniter Valve               | CLOSED      |
| N2O Main Valve                  | CLOSED      |
| IPA Main Valve                  | CLOSED      |
| Ignition Detector               | MONITORING  |

---

### #2 – N2O Fill Start  
Démarrer le remplissage de N2O.  

**GSE (Ground Support Equipment)**
| Device           | State  |
| :---             | :---   |
| N2O Fill Valve   | OPENS  |

**Moteur (Rocket)**
| Device           | State  |
| :---             | :---   |
| *Aucune action* | *-* |

---

### #3 – N2O Fill Finish  
Arrêter le remplissage lorsque le réservoir est plein.  

**GSE (Ground Support Equipment)**
| Device                  | State              |
| :---                    | :---               |
| Propellant Load Cell    | DETECTS FULL TANK  |
| N2O Fill Valve          | CLOSES             |

**Moteur (Rocket)**
| Device                  | State              |
| :---                    | :---               |
| *Aucune action* | *-* |

---

### #4 – Igniter Start  
Préparer l’allumage en activant les vannes d’allumeur et l’étincelle.  

**GSE (Ground Support Equipment)**
| Device            | State          |
| :---              | :---           |
| *Aucune action* | *-* |

**Moteur (Rocket)**
| Device            | State          |
| :---              | :---           |
| N2O Igniter Valve | OPENS          |
| IPA Igniter Valve | OPENS          |
| Spark Plug        | POWERS (Burst) |

---

### #5 – Engine Start  
Déclencher le moteur et basculer sur les vannes principales.  

**GSE (Ground Support Equipment)**
| Device            | State            |
| :---              | :---             |
| *Aucune action* | *-* |

**Moteur (Rocket)**
| Device            | State            |
| :---              | :---             |
| Ignition Detector | DETECTS IGNITION |
| Spark Plug        | UNPOWERS         |
| N2O Main Valve    | OPENS            |
| IPA Main Valve    | OPENS            |

---

### #6 – Engine Finish  
Fermeture des valves de l'igniter 1 à 3 secondes après l'ouverture des valves du main.

**GSE (Ground Support Equipment)**
| Device            | State            |
| :---              | :---             |
| *Aucune action* | *-* |

**Moteur (Rocket)**
| Device            | State            |
| :---              | :---             |
| N2O Igniter Valve | CLOSES           |
| IPA Igniter Valve | CLOSES           |
