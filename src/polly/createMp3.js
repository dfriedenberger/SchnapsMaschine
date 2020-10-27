const AWS = require("aws-sdk");
const fs = require("fs");

var polly = new AWS.Polly({ region: "eu-central-1"});

var file = process.argv[2];
var audioPath = process.argv[3];



// read contents of the file
const data = fs.readFileSync(file, 'UTF-8');

// split the contents by new line
const lines = data.split(/\r?\n/);

// print all lines
lines.forEach((line) => {

    let text = line.substring(4).trim();
    let filename = line.replace(/[^A-Za-z0-9]/g, "_");

    var length = 60;
    filename = filename.length > length ? filename.substring(0, length) :  filename;


    var voice = "Vicki"; //Marlene, Vicki, Hans
    if(text.startsWith(":ES"))
    {
        text = text.substring(3).trim();
        voice = "Lucia"; //Lucia, Conchita, Enrique
    }

    console.log(text);
    var params = {
        OutputFormat: "mp3", 
        SampleRate: "8000", 
        Text: text, 
        TextType: "text", 
        VoiceId: voice 
       };
       polly.synthesizeSpeech(params, function(err, data) {
         if (err) console.log(err, err.stack); // an error occurred
         else  
         {
            if (data.AudioStream instanceof Buffer) {
                fs.writeFile(audioPath+"/"+filename+".mp3", data.AudioStream, function(err) {
                    if (err) {
                        return console.log(err)
                    }
                    console.log("The file was saved!")
                })
            }
         }
       });



});



