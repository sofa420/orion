#include "threadman.h"

ThreadManager::ThreadManager(ChannelManager *c){
    cman = c;
    polling = false;
    threadsrunning = false;
}

ThreadManager::~ ThreadManager(){
    std::cout << "Destroyer: ThreadManager\n";
    polling = false;
    this->wait_for_threads();
    this->complete_threads();
}

void t_check(Channel *channel, ChannelManager *cman){
	if (channel){
		std::string uristr = TWITCH_URI;
		uristr += "/streams/";
		uristr += channel->getUriName();

		std::string str = conn::Get(uristr.c_str());	

        cman->parseStreamDataToJSON(str);
	}
}

void t_checkAll(ChannelManager *cman){
    std::string url = TWITCH_URI;
    url += "/streams?limit=100&channel=";
    for(size_t i = 0; i < cman->getChannels()->size(); i++){
        if (i > 0){
            url += ",";
        }
        url += cman->getChannels()->at(i)->getUriName();
    }
    cman->parseStreams(conn::Get(url));
}

void t_update(Channel *channel, ChannelManager *cman){
	if (channel){
		std::string uristr = TWITCH_URI;
		uristr += "/channels/";
		uristr += channel->getUriName();
		std::string str = conn::Get(uristr.c_str());	

        cman->parseChannelDataToJSON(str);
	}
}

void t_add(Channel *channel, ChannelManager *cman){
    if (channel){
        std::string uristr = TWITCH_URI;
        uristr += "/streams/" + channel->getUriName();

        int val = cman->parseStreamDataToJSON(conn::Get(uristr.c_str()));
        if (val == -1){         //Channel not found
            cman->channelNotFound(channel);
        }
        else if (val == 0){     //Channel found, stream offline. Get channel data
           uristr = TWITCH_URI;
           uristr += "/channels/" + channel->getUriName();
           cman->parseChannelDataToJSON(conn::Get(uristr.c_str()));
        }
    }
}

void t_getfile(std::string uri, std::string path, Channel* channel){
    //std::cout << "uri: " << uri << " path: " << path << "\n";
	if (uri.empty()){
		std::cout << "No url set for file!\n";
		return;
	}
	if (path.empty()){
		std::cout << "No url set for file!\n";
		return;
	}
	conn::GetFile(uri.c_str(),path.c_str());
    if (channel){
        channel->iconUpdated();
    }
}

void t_poll(ThreadManager *tman){
    while (tman->isPolling()){
        //cman->checkStreams(true);
        for (int i=0; i < 30 && tman->isPolling(); i++)
            usleep(1000);
    }
}

void ThreadManager::complete_threads(){
    try{
        for (unsigned int i=0; i < threads.size(); i++){
            threads.at(i).join();
        }

        threads.clear();

    } catch(std::exception& e){
        std::cout << "\nError: "<< e.what() << "\n";
    }
}

void t_complete_async(std::vector<std::thread> *threads,ThreadManager *tman){
    try{
        for (unsigned int i=0; i < threads->size(); i++){
                threads->at(i).join();
        }
        std::cout << "\nTHREAD NUM: " << threads->size() << std::endl;
        threads->clear();
    } catch(std::exception& e){
        std::cout << e.what();
    }

    tman->finish_threads();
}

void ThreadManager::complete_threads_async(){
    threadsrunning = true;
    std::thread t(t_complete_async,&threads,this);
    t.detach();
}

void ThreadManager::update(Channel *channel){
    threads.push_back(std::thread(t_update,channel,cman));
}

void ThreadManager::add(Channel *channel){
    threads.push_back(std::thread(t_add,channel,cman));
}

void ThreadManager::check(Channel *channel){
    threads.push_back(std::thread(t_check,channel,cman));
}

void ThreadManager::checkAll(){
    threads.push_back(std::thread(t_checkAll,cman));
}

void ThreadManager::getfile(std::string uri, std::string path, Channel* channel){
    threads.push_back(std::thread(t_getfile,uri,path, channel));
}

void ThreadManager::startPolling(){
    polling = true;
    threads.push_back(std::thread(t_poll,this));
}

bool ThreadManager::isPolling()
{
    return polling;
}

void ThreadManager::finish_threads(){
    //cman->save();
    threadsrunning = false;
}

void ThreadManager::wait_for_threads(){
    std::cout << "Waiting for threads to finish..\n";
    while (threadsrunning) usleep(500);
}