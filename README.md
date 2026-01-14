# The Last Sermon

**Engine:** Unreal Engine 5  
**Languages:** C++, Blueprints  
**Type:** Two-Player Couch Co-op Action RPG  

---

## Overview

**The Last Sermon** is a two-player local co-op action game where players choose melee or ranged characters to fight through enemies and face a final boss. The game features a rich story, interactive environments, and dynamic AI systems.

Play the game here: [Itch.io Page](https://reflexxak.itch.io/the-last-sermon)

---

## Features

### Player Mechanics
- Two-player local co-op with melee and ranged characters.  
- Light and heavy attacks for both characters.  
- Interactable campfire system to heal both players simultaneously.  
- Full keyboard, mouse, and PlayStation controller support.

### AI Systems
- Enemy AI implemented with **Behavior Trees** and **AI Perception** to chase and attack players.  
- Boss AI with multiple attack states:
  - Melee attacks  
  - Targeted magic attacks spawning fire at player location  
- Blackboard logic and modular AI tasks for decision-making and attack selection.  
- AI reacts intelligently to both melee and ranged players.  
- Navigation and pathfinding tuned with Unreal’s **NavMesh system**.

### Modular Architecture
- Actor components handle stats, attacks, and player input for scalable and reusable gameplay systems.  
- Systems designed for future expansion of enemies and characters.

### Environment & Narrative
- Variety of environments, maps, and enemies leading to a climactic boss fight.  
- Cinematics to enhance narrative and visual storytelling.  
- Full dialogue between main characters and NPCs.

---

## Screenshots / Video
- [Gameplay Video](https://www.youtube.com/watch?v=C44VqVxoOPE)

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/fizzyriksta91/TheLastSermon.git
2. Open the project in Unreal Engine 5.6 through your chosen IDE.
3. Compile C++ code and open in editor.
