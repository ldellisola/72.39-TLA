namespace Lib;

public static class Helpers {
  public static IMediaInfo OpenVideo(string path){
    
    return FFmpeg.GetMediaInfo(path).Result;
    }

  public static IVideoStream SetSize(this IVideoStream stream, string size){
    var list = size.ToLowerInvariant().Split('x').ToList().ConvertAll(t=> int.Parse(t));

     return stream.SetSize(list[0], list[1]);
  }
}

