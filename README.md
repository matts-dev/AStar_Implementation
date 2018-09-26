# AStar_Implementation
A very roughly coded demonstration of the concept.

![Visual](https://i.imgur.com/X9FZfsV.gif)

yellow lines: the path the agent found to its target

red lines: nodes that are upcoming in the priority queue

blue lines: nodes that have been memoized, if generated again they will not be added to the priority queue.

I didn't intend for anyone to actually try and build this, but in case someone does (or in the future I would like to), here is a basic overview of a quick setup. Note that I only every built with window's visual studio. To avoid manually setting up the linker settings, copy (and rename) the provided ```copy.suo``` file into the hidden directory `/BasicAI_SFML/.vs/BasicAI_SFML/v15/` and name it ```.suo```. That is the visual studio solution user options file. It contains the include paths etc. I haven't tested replacing the .suo file with this project, but it has worked with other projects. If it doesn't work, you'll need to manually add the include paths. https://www.sfml-dev.org/tutorials/2.5/start-vc.php 

You'll also need to find the sfml .dlls and move those to the folder of your built .exe


Texture's source: https://opengameart.org/content/dungeon-crawl-32x32-tiles
