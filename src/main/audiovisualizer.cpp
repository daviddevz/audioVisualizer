/* Project includes*/
#include "audioVisualizer.hpp"

void AudioVisualizer::loadWindow(){
    window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle,
    sf::Style::Titlebar | sf::Style::Close);
}
void AudioVisualizer::loadFont(){
    if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
        throw std::runtime_error("Failed to load font");
    }
}

UploadButton AudioVisualizer::loadUploadButton(){
    UploadButton myUploadButton(sf::Vector2f(windowWidthFloat, windowHeightFloat), 
    font, uploadButtonText, sf::Vector2f(buttonWidth, buttonHeight), fontSize);
    return myUploadButton;
}

ProgressBar AudioVisualizer::loadProgressBar(){
    ProgressBar loadingBar(loadingStages, sf::Vector2f(windowWidthFloat, windowHeightFloat),
    sf::Vector2f(buttonWidth, (buttonHeight * 0.5)));
    return loadingBar;
}
void AudioVisualizer::startAudioVisualizer(){
    loadWindow();
    loadFont();
    UploadButton uploadButton = loadUploadButton();
    ProgressBar progressBar = loadProgressBar();

    // Start the app loop
    while (window.isOpen()){
        ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window

        sf::Event event;// Process events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){ // Close window: exit
                window.close();
            }

            // Check for button clicks
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                if (uploadButton.isClicked(window) && uploadButton.clearButton == false){
                    uploadButton.clearButton = true;
                    sf::WindowHandle windowHandle = window.getSystemHandle();
                    AudioFileHandling myAudioFile;
                    myAudioFile.openFileDir(windowHandle); // Opens file directory
                    //std::cout<<"File Path in main.cpp"<<ouputFilePath<<std::endl;
                }
            }

            else if (event.type == sf::Event::MouseMoved){
                uploadButton.updateColor(window); //update button color if mouse hover over button
            }
        }
 
        window.clear(sf::Color(111, 143, 175)); // Clear screen with Denim color

        // Conditional statement that controls how button is displayed on screen
        if (uploadButton.clearButton == false){
            uploadButton.draw(window); // Draws button
        }
        else if (progressBar.clearProgressBar == false){
            sf::Color backgroundColor(240, 255, 255);
            sf::Color maskedColor(0, 255, 255, 50);
            // backgroundShape Design
            progressBar.shapeDesign(backgroundColor, maskedColor);

            // setup text
            progressBar.setText(font, (fontSize - 20));

            progressBar.updateProgressBar(2, "Processing ...");
            progressBar.draw(window);
        }
        
        window.display(); // Update the window
    }
}

