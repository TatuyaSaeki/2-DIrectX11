//===========================================================================
//! @file   filter_manager.cpp
//!	@brief	�`��Ɏg�p����t�B���^�[�Ǘ�
//===========================================================================

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool FilterManager::initialize()
{
    return true;
}

//---------------------------------------------------------------------------
//! �`��
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
//! ���
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
//! �A�v�����Ŏg�p���Ă���t�B���^�J��
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
//! �g�p����t�B���^�ݒ�
//---------------------------------------------------------------------------
void FilterManager::setFilter(FilterType filterType)
{
    // �ۑ��p�t�B���^�ɒǉ�
    addFilter(filterType);

    if(appUsefilters_.size() == 0) {
        for(auto& filter : filters_) {
            if(filter->getFilterType() == filterType) {
                // ToneMapping�Ȃ��Ԍ��ɔz�u(���̌�ɏ������邩��������A�܂���������)
                if(filterType == FilterType::ToneMapping) {
                    appUsefilters_.emplace_back(filter);
                    isHDR_ = false;
                }
                else {
                    // �t�B���^�[�ǉ�
                    appUsefilters_.emplace_front(filter);
                }
                return;
            }
        }
    }

    // �����t�B���^�����łɒǉ�����Ă���I��
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
//! �g�p����t�B���^����
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
//! �t�B���^�ǉ�
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
//! �K�E�V�A���ڂ����t�B���^�쐬
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
//! �g�[���}�b�s���O�t�B���^�쐬
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
//! HDR�����ǂ���
//---------------------------------------------------------------------------
bool FilterManager::isHDR() const
{
    return isHDR_;
}