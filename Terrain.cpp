#include "Terrain.h"

TerrainChunk::TerrainChunk(int size, int posX, int posY, float offset,  PerlinNoiseGenerator* pn) : size(size), posX(posX), posY(posY)
{
    
    heightMap = new float*[size];
    
    int currentIndice = 0;

    for(int x = 0; x < size; x++)
    {
        heightMap[x] = new float[size];
        
        for(int y = 0; y < size; y++)
        {
            
            
            
            
            float coordX = (posX * (size-1) + x);
            float coordY = (posY * (size-1) + y);
            
            
            float height = pn->getHeightAt(coordX, coordY);
            
            
			if (rand() % 20 == 0)
            {
                
                entities.push_back(new Seaweed(glm::vec3(coordX, height+1, coordY)));
                
            }
			// randomly place a rock ***** Need condition so they dont sit on a peak (looks weird) *****
			else if (rand() % 57 == 0)
			{
				entities.push_back(new Rock(glm::vec3(-coordX, -height, -coordY)));

			}
            
            heightMap[x][y] = height;
            
            
            
            vertices.push_back({coordX, height, coordY });
            if(x > 0 && y > 0)
            {
                indices.push_back(currentIndice);
                indices.push_back(currentIndice-1);
                indices.push_back(currentIndice-size-1);
                
                indices.push_back(currentIndice);
                indices.push_back(currentIndice-size);
                indices.push_back(currentIndice-size-1);
                
            }
            
            currentIndice++;
        }
    }
    
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Buffer object data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Apply translation to model matrix
    
    // Apply scale to model matrix
    
    // Apply rotations to model matrix
    
    
    
    
    
    // Compile and load shaders
    shader = new Shader("res/shaders/terrain.vs", "res/shaders/terrain.fs");
    
    
}

int TerrainChunk::getPosX()
{
    return posX;
}


int TerrainChunk::getPosY()
{
    return posY;
}

float TerrainChunk::getHeightAt(int x, int y)
{
    if(x < 0 || y < 0 || x >= size || y >= size)
    {
        return 0;
    }
    
    return heightMap[x][y];
}

void TerrainChunk::setHeightAt(int x, int y, float height)
{
    if(x < 0 || y < 0 || x >= size || y >= size)
    {
        return;
    }
    
    heightMap[x][y] = height;
    
}

void TerrainChunk::render(glm::mat4 view, glm::mat4 projection)
{
    shader->use();
    
    // Broadcast the uniform values to the shaders
    GLuint modelLoc = glGetUniformLocation(shader->program, "model");
    GLuint viewLoc = glGetUniformLocation(shader->program, "view");
    GLuint projectionLoc = glGetUniformLocation(shader->program, "projection");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Draw object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    
    for(auto entity : entities)
    {
        entity->render(view, projection);
    }
    
}

Terrain::Terrain()
{
    
    Config config("res/config/Terrain.config");
    
    size = config.getConfig()->getInt("size");
    renderDistance = config.getConfig()->getInt("renderDistance");
    
    chunks = new TerrainChunk**[size];
    
    ConfigSection* generatorConfig = config.getConfig()->getSection("generator");
    
    double persistence = generatorConfig->getDouble("persistence");
    double frequency = generatorConfig->getDouble("frequency");
    double amplitude = generatorConfig->getDouble("amplitude");
    int octaves = generatorConfig->getInt("octaves");
    
    PerlinNoiseGenerator* perlin = new PerlinNoiseGenerator(persistence, frequency, amplitude, octaves, rand());
    
    ConfigSection* chunkConfig = config.getConfig()->getSection("chunk");
    
    pointsPerChunk = chunkConfig->getInt("pointsPerChunk");
    
    
    for(int x = 0; x < size; x++)
    {
        chunks[x] = new TerrainChunk*[size];
        
        for(int y = 0; y < size; y++)
        {
            chunks[x][y] = new TerrainChunk(pointsPerChunk, x, y, (float)size/2.0f ,perlin);
        }
    }
    
    std::cout << "terrain finished" << std::endl;
}


int Terrain::getSize()
{
    return size;
}

int Terrain::getPointsPerChunk()
{
    return pointsPerChunk;
}

TerrainChunk* Terrain::getChunkAt(int posX, int posY)
{
    if(posX < 0 || posY < 0 || posX >= size || posY >= size)
    {
        return nullptr;
    }
    
    return chunks[posX][posY];
}
TerrainChunk* Terrain::getChunkAtReal(int posX, int posY)
{
    return getChunkAt(posX/(pointsPerChunk-1), posY / (pointsPerChunk-1));
}

float Terrain::getHeightAt(int x, int y)
{
    
    int relX = x % (pointsPerChunk - 1);
    int relY = y % (pointsPerChunk - 1);
    
    
    //chunk position
    int chunkX = x / (pointsPerChunk-1);
    int chunkY = y / (pointsPerChunk-1);
    
    TerrainChunk* chunk;
    if((chunk = getChunkAt(chunkX,chunkY)) != nullptr)
    {
        //position relative to the chunk
        
        return chunk->getHeightAt(relX, relY);
    }
    else
    {
        return 0;
    }
}

void Terrain::setHeightAt(int x, int y, float  height)
{
    //chunk position
    int chunkX = x % pointsPerChunk;
    int chunkY = y % pointsPerChunk;
    
    TerrainChunk* chunk;
    if((chunk = getChunkAt(x,y)) != nullptr)
    {
        //position relative to the chunk
        int relX = x - chunkX * pointsPerChunk;
        int relY = y - chunkY * pointsPerChunk;
        
        chunk->setHeightAt(relX, relY, height);
    }
    else
    {
        return;
    }
}


void Terrain::render(glm::vec3 position, glm::mat4 view, glm::mat4 proj)
{
    TerrainChunk* chunk = getChunkAt(-position.x/(pointsPerChunk-1), -position.z/(pointsPerChunk-1));
    if(chunk != nullptr)
    {
        
        int minX = chunk->getPosX()-renderDistance;
        int minY = chunk->getPosY()-renderDistance;
        
        int maxX = chunk->getPosX()+renderDistance;
        int maxY = chunk->getPosY()+renderDistance;
        
        if(minX < 0)
        {
            minX = 0;
        }
        if(minY < 0)
        {
            minY = 0;
        }
        
        if(maxX > size-1)
        {
            maxX = size-1;
        }
        
        if(maxY > size-1)
        {
            maxY = size-1;
        }
        
        
        for(int x = minX; x <= maxX; x++)
        {
            for(int y = minY; y <= maxY; y++)
            {
                getChunkAt(x,y)->render(view,proj);
            }
        }
        
    }
    else
    {
        for(int x = 0; x < size; x++)
        {
            for(int y = 0; y < size; y++)
            {
                getChunkAt(x,y)->render(view,proj);
            }
        }
    }
    
}