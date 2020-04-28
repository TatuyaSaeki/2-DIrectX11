//===========================================================================
//! @file   filter_manager.cpp
//!	@brief	描画に使用するフィルター管理
//===========================================================================

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool FilterManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void FilterManager::begin()
{
    for(auto& filter : appUsefilters_) {
        if(!filter.expired()) {
            filter.lock()->begin();
        }
    }
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void FilterManager::cleanup()
{
    appFilterCleanup();

    for(auto& filter : filters_) {
        filter->finalize();
        filter.reset();
    }

    filters_.clear();
}

//---------------------------------------------------------------------------
//! アプリ側で使用しているフィルタ開放
//---------------------------------------------------------------------------
void FilterManager::appFilterCleanup()
{
    for(auto& filter : appUsefilters_) {
        if(!filter.expired()) {
            filter.lock()->finalize();
        }
        filter.reset();
    }

    appUsefilters_.clear();
}

//---------------------------------------------------------------------------
//! 使用するフィルタ設定
//---------------------------------------------------------------------------
void FilterManager::setFilter(FilterType filterType)
{
    // 保存用フィルタに追加
    addFilter(filterType);

    if(appUsefilters_.size() == 0) {
        for(auto& filter : filters_) {
            if(filter->getFilterType() == filterType) {
                // ToneMappingなら一番後ろに配置(この後に処理するかもだから、また直すかも)
                if(filterType == FilterType::ToneMapping) {
                    appUsefilters_.emplace_back(filter);
                    isHDR_ = false;
                }
                else {
                    // フィルター追加
                    appUsefilters_.emplace_front(filter);
                }
                return;
            }
        }
    }

    // 同じフィルタがすでに追加されてたら終了
    for(auto& appFilter : appUsefilters_) {
        if(appFilter.lock()->getFilterType() == filterType) {
            return;
        }
    }

    for(auto& filter : filters_) {
        if(filter->getFilterType() == filterType) {
            if(filterType == FilterType::ToneMapping) {
                appUsefilters_.emplace_back(filter);
                isHDR_ = false;
                break;
            }
            else {
                appUsefilters_.emplace_front(filter);
                break;
            }
        }
    }
}

//---------------------------------------------------------------------------
//! 使用するフィルタ解除
//---------------------------------------------------------------------------
void FilterManager::removeFilter(FilterType filterType)
{
    auto itr = appUsefilters_.begin();

    while(itr != appUsefilters_.end()) {
        if(itr->lock()->getFilterType() == filterType) {
            if(itr->lock()->getFilterType() == FilterType::ToneMapping) {
                isHDR_ = true;
            }
            itr = appUsefilters_.erase(itr);
        }
        else {
            ++itr;
        }
    }
}

//---------------------------------------------------------------------------
//! フィルタ追加
//---------------------------------------------------------------------------
void FilterManager::addFilter(FilterType filterType)
{
    // naos
    for(auto& filter : filters_) {
        if(filter->getFilterType() == filterType) {
            return;
        }
    }

    std::unique_ptr<Filter> filter;

    switch(filterType) {
        case FilterType::GaussianBlur:
            filter.reset(createGaussianBlur());
            break;
        case FilterType::ToneMapping:
            filter.reset(createToneMapping());
            break;
        default:
            break;
    }

    if(!filter) {
        return;
    }

    //
    if(filterType == FilterType::ToneMapping) {
        filters_.emplace_back(filter.release());
    }
    else {
        filters_.emplace_back(filter.release());
    }
}

//---------------------------------------------------------------------------
//! ガウシアンぼかしフィルタ作成
//---------------------------------------------------------------------------
Filter* FilterManager::createGaussianBlur()
{
    std::unique_ptr<Filter> filter(new FilterGlare());

    if(!filter->initialize()) {
        return false;
    }

    return filter.release();
}

//---------------------------------------------------------------------------
//! トーンマッピングフィルタ作成
//---------------------------------------------------------------------------
Filter* FilterManager::createToneMapping()
{
    std::unique_ptr<Filter> filter(new ToneMapping());

    if(!filter->initialize()) {
        return false;
    }

    return filter.release();
}

//---------------------------------------------------------------------------
//! HDR中かどうか
//---------------------------------------------------------------------------
bool FilterManager::isHDR() const
{
    return isHDR_;
}