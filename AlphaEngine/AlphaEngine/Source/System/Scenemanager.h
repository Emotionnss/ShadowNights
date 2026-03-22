class BaseScene;

class SceneManager {
public:
    static void ChangeScene(BaseScene* pNewScene);

    static void Update();

    static void Draw();

    static void Exit();

    static BaseScene* GetCurrentScene() { return pCurrentScene; }

private:
    static BaseScene* pCurrentScene;
    static BaseScene* pNextScene;
};