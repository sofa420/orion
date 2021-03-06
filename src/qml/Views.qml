import QtQuick 2.5
import "components"
import "styles.js" as Styles


Rectangle {
    property int selection
    property alias playerView: loader.item
    id: root

    signal requestSelectionChange(int index)

    color: Styles.bg

    onSelectionChanged: {
        searchView.visible = false
        favouritesView.visible = false
        gamesView.visible = false
        featuredView.visible = false
        playerView.visible = false
        settingsView.visible = false
        vodsView.visible = false
        webView.visible = false

        switch (selection){

        //Search
        case 0:
            searchView.visible = true
            gradient.parent = searchView
            searchView.focusInput()
            break

        //Featured
        case 1:
            featuredView.visible = true
            gradient.parent = featuredView
            break

        //Fav
        case 2:
            favouritesView.visible = true
            gradient.parent = favouritesView
            break

        //Games
        case 3:
            gamesView.visible = true
            gradient.parent = gamesView
            break

        //Vods
        case 4:
            vodsView.visible = true
            gradient.parent = vodsView
            break

        //Player
        case 5:
            playerView.visible = true
            break

        //Settings
        case 6:
            settingsView.visible = true
            break

        case 7:
            webView.visible = true
            break
        }
    }

    SearchView {
        id: searchView
        visible: false
    }

    FeaturedView{
        id: featuredView
        visible: false
    }

    FavouritesView{
        id: favouritesView
        visible: false
    }

    GamesView {
        id: gamesView
        visible: false
    }

    VodsView {
        id: vodsView
        visible: false
    }

    OptionsView{
        id: settingsView
        visible: false
    }

    WebView{
        id: webView
        visible: false
    }

    Loader {
        id: loader
        //visible: false
        source: backend_mpv ? "PlayerView.qml" : "GstPlayer.qml"
        onLoaded: {
            item.parent = root
            item.visible = false
        }
    }

    //The gradient that is applied to each view
    GradientBottom {
        id: gradient
        parent: searchView
    }

}
