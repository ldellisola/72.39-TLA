namespace Lib;

public static class Helpers {
  public static IMediaInfo OpenVideo(string path){
    
    return FFmpeg.GetMediaInfo(path).Result;
    }

  public static IVideoStream SetSize(this IVideoStream stream, string size){
    size = size.ToLowerInvariant();

    List<int> list;
  
   try{
    list = size.Split('x').ToList().ConvertAll(t=> int.Parse(t));
    if(list.Count != 2)
    throw new Exception();
   }
   catch
   {
     throw new Exception($"The parameter 'size' does not accept the string '{size}'");
   }



     return stream.SetSize(list[0], list[1]);
  }
}

