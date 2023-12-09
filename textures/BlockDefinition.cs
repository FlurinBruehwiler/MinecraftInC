namespace RayLib3dTest;

public class BlockDefinition
{
    public string LeftTexture { get; set; } = null!;
    public string RightTexture { get; set; } = null!;
    public string TopTexture { get; set; } = null!;
    public string BottomTexture { get; set; } = null!;
    public string BackTexture { get; set; } = null!;
    public string FrontTexture { get; set; } = null!;
    public required string Name { get; set; }
    public required ushort Id { get; set; }

    public string SideTexture
    {
        set
        {
            LeftTexture = value;
            RightTexture = value;
            BackTexture = value;
            FrontTexture = value;
        }
    }
    
    public string Texture
    {
        set
        {
            LeftTexture = value;
            RightTexture = value;
            BackTexture = value;
            FrontTexture = value;
            TopTexture = value;
            BottomTexture = value;
        }
    }
}